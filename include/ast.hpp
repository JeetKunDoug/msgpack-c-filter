#ifndef msgpack_filter_ast_hpp
#define msgpack_filter_ast_hpp

struct Ast {
    // TODO - Actually do something useful with the filter.
    const std::string &filter;
    Ast(const std::string& filter): filter(filter)  {}
};

#endif
