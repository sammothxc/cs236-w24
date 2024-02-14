class Scanner {
private:
    vector<Token> tokens;
    int line = 1;

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isSpace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    void skipWhitespace(const string& input, size_t& pos) {
        while (pos < input.size() && isSpace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
    }

    void parseToken(const string& input, size_t& pos) {
        skipWhitespace(input, pos);

        if (pos >= input.size()) return;

        char currentChar = input[pos];

        switch (currentChar) {
            case ':':
                if (pos + 1 < input.size() && input[pos + 1] == '-') {
                    tokens.emplace_back(COLON_DASH, ":-", line);
                    pos += 2;
                } else {
                    tokens.emplace_back(COLON, ":", line);
                    pos++;
                }
                break;
            case '(':
                tokens.emplace_back(LEFT_PAREN, "(", line);
                pos++;
                break;
            case ')':
                tokens.emplace_back(RIGHT_PAREN, ")", line);
                pos++;
                break;
            case ',':
                tokens.emplace_back(COMMA, ",", line);
                pos++;
                break;
            case '.':
                tokens.emplace_back(PERIOD, ".", line);
                pos++;
                break;
            case '?':
                tokens.emplace_back(Q_MARK, "?", line);
                pos++;
                break;
            default:
                if (isAlpha(currentChar)) {
                    string id;
                    while (pos < input.size() && (isAlpha(input[pos]) || isDigit(input[pos]))) {
                        id += input[pos];
                        pos++;
                    }
                    if (id == "Schemes") tokens.emplace_back(SCHEMES, id, line);
                    else if (id == "Facts") tokens.emplace_back(FACTS, id, line);
                    else if (id == "Rules") tokens.emplace_back(RULES, id, line);
                    else if (id == "Queries") tokens.emplace_back(QUERIES, id, line);
                    else tokens.emplace_back(ID, id, line);
                } else if (currentChar == '\'') {
                    string str;
                    pos++; // skip opening quote
                    while (pos < input.size() && input[pos] != '\'') {
                        str += input[pos];
                        pos++;
                    }
                    if (pos < input.size()) pos++; // skip closing quote
                    tokens.emplace_back(STRING, str, line);
                } else {
                    tokens.emplace_back(UNKNOWN, string(1, currentChar), line);
                    pos++;
                }
        }
    }

public:
    Scanner(const string& input) {
        size_t pos = 0;
        while (pos < input.size()) {
            parseToken(input, pos);
        }
        tokens.emplace_back(EOF_TOKEN, "", line);
    }

    const vector<Token>& getTokens() const {
        return tokens;
    }
};