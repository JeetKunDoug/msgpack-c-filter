#ifndef msgpack_filter_expression_hpp
#define msgpack_filter_expression_hpp

#include "extractor.hpp"
#include "expression_node.hpp"

// Expressions will take an AST and build (internally):
// 1) A ValueExtractor - the thing that knows what fields in a msgpack record to extract based on the AST, and how to extract them from the raw data
// 2) A tree of operators that make up the filter expression, which should have a reference to the value extractor so they can retrieve the data they need

struct Expression {
    Extractor* extractor;
    ExpressionNode<bool>* root;
    Ast ast;
    Expression(const Ast& expr) : extractor(new Extractor(expr)), ast(expr) {
        // TODO: Actually use the AST to build the root expression
        FieldValue<double>* left = new FieldValue<double>("long");
        ConstantValue<double>* right = new ConstantValue<double>(0.775947);
        root = new GteOperator<double>(left, right);
    }
    
    void clear_values() {
        this->root->clear();
    }
    
    bool evaluate(const std::string& data) {
        clear_values();
        extractor->extract(data, root);
        return this->root->evaluate();
    }
};

#endif
