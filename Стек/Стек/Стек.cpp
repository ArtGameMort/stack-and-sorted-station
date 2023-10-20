#include <iostream>
#include <cmath>
#include <string>
#include <vector>

template<typename T>
class stack_dynamic {
public:
    stack_dynamic();  // Конструктор по умолчанию

    ~stack_dynamic() { delete[] _data; }  // Деструктор

    stack_dynamic(const stack_dynamic<T>& other);  // Конструктор копирования

    stack_dynamic<T>& operator=(const stack_dynamic<T>& other);  // Оператор присваивания

    void push(T x);   // Метод добавления элемента x на верхушку стека

    T pop();  // Метод удаления элемента из стека

    T top() const;   // Посмотреть на верхний элемент стека, не удаляя его

    bool is_empty() const;  // Проверить является ли стек пустым

    template<typename C>
    friend std::ostream& operator<<(std::ostream& out, const stack_dynamic<C>& s);  // Перегрузка оператора вывода в поток

private:
    T* _data;  // Динамический массив элементов
    size_t _cap;  // Вместимость стека
    size_t _size; // Размер стека
};

template<typename T>
T stack_dynamic<T>::top() const {
    if (_size == 0) {
        throw std::runtime_error("Stack is empty.");
    }
    return _data[_size - 1];
}

template<typename T>
stack_dynamic<T>::stack_dynamic(const stack_dynamic<T>& other) : _data(new T[other._cap]), _cap(other._cap),
_size(other._size) {
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }
}

template<typename T>
stack_dynamic<T>& stack_dynamic<T>::operator=(const stack_dynamic<T>& other) {
    if (this == &other) {
        return *this;
    }
    delete[] _data;
    _cap = other._cap;
    _size = other._size;
    _data = new T[_cap];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const stack_dynamic<T>& s) {
    for (size_t i = 0; i < s._size; ++i) {
        out << s._data[i] << ' ';
    }
    return out;
}

template<typename T>
stack_dynamic<T>::stack_dynamic() : _data(nullptr), _size(0), _cap(0) {}

template<typename T>
void stack_dynamic<T>::push(T x) {
    if (_size + 1 >= _cap) {
        T* tmp = new T[_size];
        for (size_t i = 0; i < _size; ++i) {
            tmp[i] = _data[i];
        }

        delete[] _data;

        if (_cap == 0) _cap = 1;
        else _cap *= 2;

        _data = new T[_cap];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = tmp[i];
        }
        delete[] tmp;
    }
    _data[_size++] = x;
}

template<typename T>
T stack_dynamic<T>::pop() {
    if (_size == 0) {
        throw std::runtime_error("Stack is empty\n");
    }
    T top = _data[_size - 1];
    --_size;
    double l = log2(_size);
    if (floor(l) == ceil(l)) {
        T* tmp = new T[_size];
        for (size_t i = 0; i < _size; ++i) {
            tmp[i] = _data[i];
        }
        delete[] _data;
        _cap = _size;
        _data = new T[_cap];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = tmp[i];
        }
        delete[] tmp;
    }
    return top;
}

template<typename T>
bool stack_dynamic<T>::is_empty() const { return _size == 0; }

/**
 *
 * @param lhs
 * @param rhs
 * @return true, если приоритет операции lhs выше, чем операции rhs.
 */
bool isPriorityHigher(bool lhs, bool rhs) {
    return ((lhs == '*' || lhs == '/') && (rhs == '-' || rhs == '+'));
}

/**
 * Перевод из инфиксной нотации в постфиксную с помощью алгоритма Дейкстры
 * @return строка в постфиксной нотации.
 */
std::vector<std::string> infixToPostfix(const std::string& math_equation) {
    stack_dynamic<char> s;
    std::vector<std::string> res;
    auto isOperation = [](char sym) { return sym == '+' || sym == '-' || sym == '*' || sym == '/' || sym == '^'; };
    for (size_t i = 0; i < math_equation.length(); ++i) {
        if (isdigit(math_equation[i])) {
            size_t j = i;
            while (j < math_equation.length() && isdigit(math_equation[j])) ++j;
            res.push_back(math_equation.substr(i, j - i));
            i = j - 1;
        }
        else if (isOperation(math_equation[i])) {
            while (!s.is_empty() && isPriorityHigher(s.top(), math_equation[i])) {
                res.push_back(std::string(1, s.pop()));
            }
            s.push(math_equation[i]);
        }
        else if (math_equation[i] == '(') {
            s.push('(');
        }
        else if (math_equation[i] == ')') {
            while (!s.is_empty() && s.top() != '(') {
                res.push_back(std::string(1, s.pop()));
            }
            s.pop();
        }
    }
    while (!s.is_empty()) res.push_back(std::string(1, s.pop()));
    return res;
}

double parseMathEquation(const std::vector<std::string>& postfix) {
    stack_dynamic<double> s;
    for (const auto& item : postfix) {
        if (!isdigit(item[0])) {
            auto rhs = s.pop();
            auto lhs = s.pop();
            switch (item[0]) {
            case '-': s.push(lhs - rhs); break;
            case '*': s.push(lhs * rhs); break;
            case '/': s.push(lhs / rhs); break;
            case '+': s.push(lhs + rhs); break;
            case '^': s.push(pow(lhs,rhs)); break;
            }
        }
        else {
            s.push(std::stod(item));
        }
    }
    return s.top();
}

int menu() {
    int command;
    std::cout << "\n";
    std::cout << "1 - Введите содержимое массива" << std::endl;
    std::cout << "2 - Вывод массива" << std::endl;
    std::cout << "3 - " << std::endl;
    std::cout << "4 - Размер" << std::endl;
    std::cout << "5 - Выход из программы" << std::endl;
    std::cin >> command;
    return command;
}

int main() {
    std::string str;
    std::cin >> str;
    std::cout << parseMathEquation(infixToPostfix(str));
    return 0;
}