#include <set>
#include "ast.hpp"
#include "expression_node.hpp"

#ifndef msgpack_filter_extractor_hpp
#define msgpack_filter_extractor_hpp

class missing_value : public std::bad_cast { };
class Extractor {
    const Ast& ast;
    std::set<std::string> expr_fields;
    msgpack::zone zone;
    std::map<std::string, msgpack::object> data;
    
public:
    std::set<std::string> extract_expr_fields(const Ast& ast) const{
        std::set<std::string> fields;
        fields.insert("long");
        return fields;
    }
    
    Extractor(const Ast& ast): ast(ast), expr_fields(extract_expr_fields(ast)) {
    }

    void extract(const std::string& data, ExpressionNode<bool>* root);
};

void Extractor::extract(const std::string& data, ExpressionNode<bool>* root) {
    zone.clear();
    msgpack::object obj = msgpack::unpack(zone, data.data(), data.size());
    // Stolen from https://github.com/msgpack/msgpack-c/blob/master/include/msgpack/object.hpp#L685
    msgpack::object_kv *p(obj.via.map.ptr);
    for (msgpack::object_kv* const pend(obj.via.map.ptr + obj.via.map.size); p < pend; ++p) {
        std::string key = p->key.as<std::string>();
        if (this->expr_fields.find(key) != this->expr_fields.end()){
            // TODO: When we actually parse the AST for this, store which nodes in the expression tree actually care about each key
            // Then, we can just push to the nodes that need the data rather than all nodes.
            root->set_value(key, &(p->val));
        }
    }
}


#endif
