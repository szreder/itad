#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

class Custom {
};

typedef std::variant <int, double, std::string> Value;
// see what happens with this Value definition :)
// typedef std::variant <int, double, std::string, Custom> Value;

enum class Operation : uint8_t {
	Plus,
	Minus,
	Multiply,
	Divide,
	Modulo,
};

template <typename T>
Value executeOperation(Operation op, const Value &left, const Value &right, typename std::enable_if <std::is_integral <T>::value, T>::type * = nullptr)
{
	static const std::unordered_map <Operation, std::function <T (const T &, const T &)> > Ops {
		{Operation::Plus, std::plus <T> {}},
		{Operation::Minus, std::minus <T> {}},
		{Operation::Multiply, std::multiplies <T> {}},
		{Operation::Divide, std::divides <T> {}},
		{Operation::Modulo, std::modulus <T> {}},
	};

	return Ops.at(op)(std::get<T>(left), std::get<T>(right));
}

template <typename T>
Value executeOperation(Operation op, const Value &left, const Value &right, typename std::enable_if <std::is_floating_point <T>::value, T>::type * = nullptr)
{
	static const std::unordered_map <Operation, std::function <T (const T &, const T &)> > Ops {
		{Operation::Plus, std::plus <T> {}},
		{Operation::Minus, std::minus <T> {}},
		{Operation::Multiply, std::multiplies <T> {}},
		{Operation::Divide, std::divides <T> {}},
	};

	return Ops.at(op)(std::get<T>(left), std::get<T>(right));
}

template <typename T>
Value executeOperation(Operation op, const Value &left, const Value &right, typename std::enable_if <std::is_same <T, std::string>::value, T>::type * = nullptr)
{
	static const std::unordered_map <Operation, std::function <T (const T &, const T &)> > Ops {
		{Operation::Plus, std::plus <T> {}},
	};

	return Ops.at(op)(std::get<T>(left), std::get<T>(right));
}

Value executeOperation(Operation op, Value left, Value right)
{
	if (left.index() != right.index()) {
		if (std::holds_alternative<int>(left) && std::holds_alternative<double>(right)) {
			left = static_cast<double>(std::get<int>(left));
		} else if (std::holds_alternative<double>(left) && std::holds_alternative<int>(right)) {
			right = static_cast<double>(std::get<int>(right));
		} else {
			abort();
		}
	}

	return std::visit([op, &left, &right](auto &&arg){
		using T = std::decay_t<decltype(arg)>;
		return executeOperation<T>(op, left, right);
	}, left);
}

std::ostream & operator << (std::ostream &os, const Value &v)
{
	std::visit([&os](auto &&arg) {
		os << arg;
	}, v);
	return os;
}

int main()
{
	Value a = 5, b = 10.4;
	Value c = executeOperation(Operation::Plus, a, b);

	std::cout << c << '\n';

	Value s1 = "foo";
	Value s2 = "bar";
	Value s3 = executeOperation(Operation::Plus, s1, s2);

	std::cout << s3 << '\n';

	return 0;
}
