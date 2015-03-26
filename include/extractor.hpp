#include <set>
#include "ast.hpp"
#include "expression_node.hpp"
#include "cmp.h"
#include "cmp_mem_access.h"

#ifndef msgpack_filter_extractor_hpp
#define msgpack_filter_extractor_hpp

class missing_value : public std::bad_cast { };
class Extractor {
    const Ast& ast;
    std::set<std::string> expr_fields;
    cmp_ctx_t cmp;
    cmp_mem_access_t ma;
    
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
    uint32_t map_size;
    char key[255];
    double value;
    cmp_mem_access_ro_init(&cmp, &ma, data.data(), data.size());
    if (!cmp_read_map(&cmp, &map_size)) {
        return; // TODO: Error Handling
    }
    for (int i=0;i<map_size;i++) {
        uint32_t string_size = 255;
        cmp_read_str(&cmp, key, &string_size);
        cmp_read_double(&cmp, &value);
        if (this->expr_fields.find(key) != this->expr_fields.end()) {
            root->set_value(key, &value);
        }
    }
}


#endif
