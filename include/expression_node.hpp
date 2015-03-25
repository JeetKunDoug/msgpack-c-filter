#ifndef msgpack_filter_operator_hpp
#define msgpack_filter_operator_hpp

#include <boost/optional.hpp>

template<typename T>
struct ExpressionNode {
public:
    virtual T evaluate() const = 0;
    virtual void clear() = 0;
    virtual void set_value(std::string key, msgpack::object* val) = 0;
};

template<typename TResult, typename TOperands>
class BinaryExpression: public ExpressionNode<TResult> {
    
protected:
    ExpressionNode<TOperands>* left;
    ExpressionNode<TOperands>* right;
    
public:
    BinaryExpression(ExpressionNode<TOperands>* left, ExpressionNode<TOperands>* right): left(left), right(right) {}
    
    virtual TResult evaluate() const = 0;
    virtual void clear() {
        left->clear();
        right->clear();
    }
    
    virtual void set_value(std::string key, msgpack::object* val) {
        left->set_value(key, val);
        right->set_value(key, val);
    }
};

template<typename T>
class GteOperator: public BinaryExpression<bool, T> {
public:
    GteOperator(ExpressionNode<T>* left, ExpressionNode<T>* right): BinaryExpression<bool, T>(left, right) {}
    
    virtual bool evaluate() const {
        return BinaryExpression<bool, T>::left->evaluate() >= BinaryExpression<bool, T>::right->evaluate();
    }
};

template<typename T>
class LteOperator: public BinaryExpression<bool, T> {
public:
    LteOperator(ExpressionNode<T>* left, ExpressionNode<T>* right): BinaryExpression<bool, T>(left, right) {}
    
    virtual bool evaluate() const {
        return BinaryExpression<bool, T>::left->evaluate() <= BinaryExpression<bool, T>::right->evaluate();
    }
};

template<typename T>
struct ConstantValue: public ExpressionNode<T> {
    const T value;
    
    ConstantValue(T val): value(val) {
    }
    
    inline virtual T evaluate() const {
        return value;
    }
    
    inline virtual void clear() {
        // noop for constant
    }
    
    inline virtual void set_value(std::string key, msgpack::object* val) {
        // noop for constant
    }
};

template<typename T>
struct FieldValue: public ExpressionNode<T> {
    const std::string field;
    boost::optional<T> value;
    
    FieldValue(const std::string fieldName): field(fieldName) {
    }
    
    inline virtual T evaluate() const {
        return *value;
    }
    
    inline virtual void clear() {
        value = boost::none;
    }
    
    inline virtual void set_value(std::string key, msgpack::object* val) {
        value = val->as<T>();
    }
};

#endif
