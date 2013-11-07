//#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/phoenix/function/adapt_function.hpp>

namespace qi  = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace ast {
    typedef double       Number;
    typedef std::wstring String;
    struct  Null { 
        friend std::wostream& operator<<(std::wostream& os, Null) { return os << L"%null%"; } 
        friend std:: ostream& operator<<(std:: ostream& os, Null) { return os <<  "%null%"; } 
    };
    typedef boost::variant<Null, Number, String> Value;

    struct IsNull {
        typedef bool result_type;
        template <typename... T>
        constexpr result_type operator()(T const&...) const { return false; }
        constexpr result_type operator()(Null const&) const { return true;  }
    };

    Value Coalesce(std::vector<Value> const& arglist) {
        for (auto& v : arglist)
            if (!boost::apply_visitor(IsNull(), v)) return v;
        //
        if (!arglist.empty()) return arglist.back(); // last is the default, even if Null
        return Null(); // Null for empty list
    }
}

BOOST_PHOENIX_ADAPT_FUNCTION(ast::Value, Coalesce_, ast::Coalesce, 1)

template <typename It, typename Skipper = qi::space_type >
struct MapFunctionParser : qi::grammar<It, ast::Value(), Skipper>
{
    MapFunctionParser() : MapFunctionParser::base_type(expr_) {
        using namespace qi;

        function_call_ = no_case[L"Coalesce"] 
            > ('(' > expr_ % ',' > ')') [ _val = Coalesce_(_1) ];

        string_ = (L'"' > *~char_('"') > L'"')
                | (L"'" > *~char_("'") > L"'"); 
        number_ = double_;
        null_   = no_case["%null%"] > attr(ast::Null());
        factor_ = null_ | string_ | double_ | function_call_;
        expr_   = factor_;

        on_error<fail> (expr_, std::cout
            << phx::val("Error! Expecting ") << _4 << phx::val(" here: \"")
            << phx::construct<std::string>(_3, _2) << phx::val("\"\n"));

        BOOST_SPIRIT_DEBUG_NODES((expr_)(factor_)(null_)(string_)(number_)(function_call_))
    }
private:
    qi::rule<It, ast::String()>         string_;
    qi::rule<It, ast::Number()>         number_;
    qi::rule<It, ast::Null()>           null_;
    qi::rule<It, ast::Value(), Skipper> factor_, expr_, function_call_;
};

int main() {
    typedef std::wstring::const_iterator It;
    MapFunctionParser<It, boost::spirit::qi::space_type> parser;

    std::wstring const input = L"Coalesce(%null%, 5, 'default')";
    It begin(input.begin()), end(input.end());

    ast::Value result;
    bool ret = phrase_parse(begin, end, parser, qi::space, result);

    std::wcout << std::boolalpha << ret << ":\t" << result << std::endl;
}
