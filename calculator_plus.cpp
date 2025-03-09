#include <iostream>
#include <memory>
#include <sstream>
#include <type_traits>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <fstream>
#include <utility>

using namespace std;

class Logger {
    static Logger* instance;
    mutex log_mutex;
    ofstream log_file;

    Logger() { log_file.open("calc_ultra.log"); }
public:
    static Logger* get() {
        static mutex m;
        lock_guard<mutex> lock(m);
        if (!instance) instance = new Logger();
        return instance;
    }

    template<typename T>
    void log(const T& msg) {
        lock_guard<mutex> lock(log_mutex);
        log_file << "[Thread " << this_thread::get_id() << "] " << msg << endl;
    }
};
Logger* Logger::instance = nullptr;

template<typename T>
struct IExpression {
    virtual ~IExpression() = default;
    virtual T eval() const = 0;
    virtual string str() const = 0;
};

template<typename T>
class Number : public IExpression<T> {
    T value;
public:
    explicit Number(T val) : value(val) {}
    T eval() const override { 
        Logger::get()->log("Number: " + to_string(value)); 
        return value; 
    }
    string str() const override { return to_string(value); }
};

template<typename T, char Op>
class Operation : public IExpression<T> {
protected:
    unique_ptr<IExpression<T>> left;
    unique_ptr<IExpression<T>> right;

public:
    Operation(unique_ptr<IExpression<T>> l, unique_ptr<IExpression<T>> r)
        : left(move(l)), right(move(r)) {}
};

template<typename T>
class Addition : public Operation<T, '+'> {
public:
    Addition(unique_ptr<IExpression<T>> l, unique_ptr<IExpression<T>> r)
        : Operation<T, '+'>(move(l), move(r)) {}
    
    T eval() const override {
        auto result = this->left->eval() + this->right->eval();
        Logger::get()->log("Addition: " + this->left->str() + " + " + this->right->str());
        return result;
    }
    
    string str() const override {
        return "(" + this->left->str() + " + " + this->right->str() + ")";
    }
};

template<typename T>
class Subtraction : public Operation<T, '-'> {
public:
    Subtraction(unique_ptr<IExpression<T>> l, unique_ptr<IExpression<T>> r)
        : Operation<T, '-'>(move(l), move(r)) {}
    
    T eval() const override {
        auto result = this->left->eval() - this->right->eval();
        Logger::get()->log("Subtraction: " + this->left->str() + " - " + this->right->str());
        return result;
    }
    
    string str() const override {
        return "(" + this->left->str() + " - " + this->right->str() + ")";
    }
};

template<typename T>
class Multiplication : public Operation<T, '*'> {
public:
    Multiplication(unique_ptr<IExpression<T>> l, unique_ptr<IExpression<T>> r)
        : Operation<T, '*'>(move(l), move(r)) {}
    
    T eval() const override {
        auto result = this->left->eval() * this->right->eval();
        Logger::get()->log("Multiplication: " + this->left->str() + " * " + this->right->str());
        return result;
    }
    
    string str() const override {
        return "(" + this->left->str() + " * " + this->right->str() + ")";
    }
};

template<typename T>
class Division : public Operation<T, '/'> {
public:
    Division(unique_ptr<IExpression<T>> l, unique_ptr<IExpression<T>> r)
        : Operation<T, '/'>(move(l), move(r)) {}
    
    T eval() const override {
        auto denominator = this->right->eval();
        if (denominator == 0) throw runtime_error("Division by zero");
        auto result = this->left->eval() / denominator;
        Logger::get()->log("Division: " + this->left->str() + " / " + this->right->str());
        return result;
    }
    
    string str() const override {
        return "(" + this->left->str() + " / " + this->right->str() + ")";
    }
};

template<typename T>
class Parser {
    istringstream input;
    char peek() { return static_cast<char>(input.peek()); }
    char get() { return static_cast<char>(input.get()); }
    
    unique_ptr<IExpression<T>> parse_primary() {
        if (isdigit(peek()) || peek() == '.') {
            T value;
            input >> value;
            return make_unique<Number<T>>(value);
        }
        if (peek() == '(') {
            get();
            auto expr = parse_expression();
            if (get() != ')') throw runtime_error("Missing closing parenthesis");
            return expr;
        }
        throw runtime_error("Invalid input");
    }
    
    unique_ptr<IExpression<T>> parse_term() {
        auto left = parse_primary();
        while (true) {
            if (peek() == '*' || peek() == '/') {
                char op = get();
                auto right = parse_primary();
                if (op == '*') 
                    left = make_unique<Multiplication<T>>(move(left), move(right));
                else 
                    left = make_unique<Division<T>>(move(left), move(right));
            } else break;
        }
        return left;
    }
    
    unique_ptr<IExpression<T>> parse_expression() {
        auto left = parse_term();
        while (true) {
            if (peek() == '+' || peek() == '-') {
                char op = get();
                auto right = parse_term();
                if (op == '+') 
                    left = make_unique<Addition<T>>(move(left), move(right));
                else 
                    left = make_unique<Subtraction<T>>(move(left), move(right));
            } else break;
        }
        return left;
    }

public:
    unique_ptr<IExpression<T>> parse(const string& expr) {
        input = istringstream(expr);
        return parse_expression();
    }
};

class IGuiFactory {
public:
    virtual void display_result(const string&) = 0;
    virtual string get_input() = 0;
    virtual ~IGuiFactory() = default;
};

class ConsoleGui : public IGuiFactory {
    mutex gui_mutex;
public:
    void display_result(const string& res) override {
        lock_guard<mutex> lock(gui_mutex);
        cout << "Result: " << res << endl;
    }
    
    string get_input() override {
        lock_guard<mutex> lock(gui_mutex);
        cout << "Enter expression: ";
        string s;
        getline(cin, s);
        return s;
    }
};

class CalculatorController {
    unique_ptr<IGuiFactory> gui;
    
    template<typename T>
    void calculate() {
        try {
            auto expr = gui->get_input();
            Parser<T> parser;
            auto ast = parser.parse(expr);
            thread([ast = move(ast), this] {
                try {
                    auto result = ast->eval();
                    this->gui->display_result(to_string(result));
                } catch (const exception& e) {
                    this->gui->display_result("Error: " + string(e.what()));
                }
            }).detach();
        } catch (const exception& e) {
            gui->display_result("Error: " + string(e.what()));
        }
    }

public:
    explicit CalculatorController(unique_ptr<IGuiFactory>&& guiImpl)
        : gui(move(guiImpl)) {}

    void run() {
        while (true) {
            calculate<double>();
        }
    }
};

int main() {
    CalculatorController controller(make_unique<ConsoleGui>());
    controller.run();
    return 0;
}
