#ifndef FUNCTT_H
#define FUNCTT_H

#include <experimental/optional>

#include <functional>
#include <string>
#include <vector>
#include <map>

namespace functt {

using std::experimental::optional;

using std::function;
using std::string;
using std::vector;
using std::map;

typedef map<string, string> varmap;
typedef map<string, function<string(string)>> funmap;

class Token {

public:
    enum TokenType {NONE, BEGIN, END, VAR, UNESC = 7, TEXT = 8, COMMENT = 16};

private:
    size_t m_begin;
    size_t m_end;
    TokenType m_type;
    string m_varname;

public:

    Token(size_t begin, size_t end, TokenType type, const string & varname): m_begin(begin), m_end(end), m_type(type), m_varname(varname) {}

    Token(size_t begin, size_t end, TokenType type) : Token(begin, end, type, "") {}

    void update_position(size_t begin, size_t end, ssize_t diff) {
        if (m_begin > begin && m_end > end) {
            m_begin += diff;
            m_end   += diff;
        }
    }

    size_t    begin()   {
        return m_begin;
    }
    size_t    end()     {
        return m_end;
    }
    TokenType type()    {
        return m_type;
    }
    string    varname() {
        return m_varname;
    }
};

class Lexer {

private:
    size_t m_pos;
    string m_str;
    string m_beg_var;

public:
    Lexer (const string &str): m_pos(0), m_str(str) {}

    bool has_next() {
        return m_pos < m_str.size();
    }

    Token next() {
        size_t begin = m_pos;
        size_t end   = m_pos;

        if (m_str[begin] == '{' && begin + 1 < m_str.size() && m_str[begin+1] == '{') {

            Token::TokenType type = Token::NONE;
            end += 2;

            if (m_beg_var.empty() || m_str[end] == '/')  {

                if (m_str[end] == '/') {
                    type = Token::END;
                    m_beg_var.clear();
                    end++;

                } else if (m_str[end] == '{') {
                    type = Token::UNESC;
                    end++;

                } else if (m_str[end] == '!') {
                    type = Token::COMMENT;
                    end++;

                } else {
                    type = Token::VAR;
                }

                string cur_var;
                size_t eot = 0;

                for (size_t i = end + 1; i < m_str.size(); ++i) {

                    if (type == Token::UNESC && m_str[i-2] == '}' && m_str[i-1] == '}' && m_str[i] == '}') {
                        cur_var = string(m_str, end, i - end - 2);
                        eot = i;
                        break;

                    } else if (type != Token::UNESC && m_str[i-1] == '}' && m_str[i] == '}') {
                        cur_var = string(m_str, end, i - end - 1);
                        eot = i;
                        break;
                    }
                }

                if (m_str[end] == '#') {
                    type = Token::BEGIN;
                    m_beg_var = cur_var;
                    end++;

                }

                eot++;
                m_pos = eot;
                return Token(begin, eot, type, cur_var);

            } else {
                end++;
                m_pos = end;
                return Token(begin, end, Token::TEXT);
            }
        }

        while (end < m_str.length()) {
            if (m_str[end-1] == '{' && m_str[end] == '{') {
                end -= 2;
                break;
            }
            end++;
        }

        end++;
        m_pos = end;
        return Token(begin, end, Token::TEXT);
    }

    string str() {
        return m_str;
    }

};

class Template {

private:
    string m_view;
    Lexer m_lexer;
    vector<Token> m_tokens;

protected:
    void replace_token(Token &tok, const string &repl) {
        size_t length = tok.end() - tok.begin();
        m_view.replace(tok.begin(), length, repl);
        ssize_t difflen = repl.length() - length;

        for (Token &it: m_tokens) {
            it.update_position(tok.begin(), tok.end(), difflen);
        }

    }

    void replace_tokens(Token &tok1, Token &tok2, string &str) {
        Token newtoken(tok1.begin(), tok2.end(), Token::NONE);
        replace_token(newtoken, str);
    }

public:
    Template (const string &view): m_lexer(view) {
    }

    static void html_encode(string& data) {
        string buffer;
        buffer.reserve(data.size()*1.05);
        for(size_t pos = 0; pos < data.size(); ++pos) {
            switch(data[pos]) {
            case '&':
                buffer.append("&amp;");
                break;
            case '<':
                buffer.append("&lt;");
                break;
            case '>':
                buffer.append("&gt;");
                break;
            case '\"':
                buffer.append("&quot;");
                break;
            case '\'':
                buffer.append("&#x27;");
                break;
            case '/':
                buffer.append("&#x2F;");
                break;
            default:
                buffer.append(&data[pos], 1);
                break;
            }
        }
        data.swap(buffer);
    }

    string render(varmap vm, funmap fm) {
        m_view = m_lexer.str();
        while (m_lexer.has_next()) {
            Token tok = m_lexer.next();
            m_tokens.push_back(tok);
        }

        optional<Token> begin;
        for (auto &tok: m_tokens) {
            if ((tok.type() & Token::VAR) == Token::VAR) {
                string replace = vm.at(tok.varname());
                if (tok.type() != Token::UNESC) html_encode(replace);
                replace_token(tok, replace);

            } else if (tok.type() == Token::BEGIN) {
                begin = tok;

            } else if (tok.type() == Token::END) {
                function<string(string)> func = fm.at(tok.varname());
                if(func && begin) {
                    string replace = func(string(m_view, begin->end(), tok.begin() - begin->end()));
                    replace_tokens(*begin, tok, replace);
                }

            } else if (tok.type() == Token::COMMENT) {
                replace_token(tok, "");
            }

        }
        return m_view;
    }

    /* Convenience functions */
    string render(varmap vm) {
        return render(vm, {});
    }

    string render() {
        return render({}, {});
    }
};

}

#endif /* FUNCTT_H */
