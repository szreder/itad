#include <cinttypes>
#include <cstring>
#include <iostream>
#include <string>
#include <variant>

typedef std::variant <int, double, std::string> Value;

enum class Operation : uint8_t {
	Plus,
	Minus,
	Multiply,
	Divide,
	Modulo,
};

template <typename T>
Value executeOperation(Operation op, const Value &left, const Value &right);


template <>
Value executeOperation<int>(Operation op, const Value &left, const Value &right)
{
	switch (op) {
		case Operation::Plus:
			return std::get<int>(left) + std::get<int>(right);
		case Operation::Minus:
			return std::get<int>(left) - std::get<int>(right);
		case Operation::Multiply:
			return std::get<int>(left) * std::get<int>(right);
		case Operation::Divide:
			return std::get<int>(left) / std::get<int>(right);
		case Operation::Modulo:
			return std::get<int>(left) % std::get<int>(right);
		default:
			abort();
	}
}

template <>
Value executeOperation<double>(Operation op, const Value &left, const Value &right)
{
	switch (op) {
		case Operation::Plus:
			return std::get<double>(left) + std::get<double>(right);
		case Operation::Minus:
			return std::get<double>(left) - std::get<double>(right);
		case Operation::Multiply:
			return std::get<double>(left) * std::get<double>(right);
		case Operation::Divide:
			return std::get<double>(left) / std::get<double>(right);
		default:
			abort();
	}
}

template <>
Value executeOperation<std::string>(Operation op, const Value &left, const Value &right)
{
	switch (op) {
		case Operation::Plus:
			return std::get<std::string>(left) + std::get<std::string>(right);
		default:
			abort();
	}
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

	if (std::holds_alternative<int>(left))
		return executeOperation<int>(op, left, right);

	if (std::holds_alternative<double>(left))
		return executeOperation<double>(op, left, right);

	if (std::holds_alternative<std::string>(left))
		return executeOperation<std::string>(op, left, right);


	abort();
}

int main()
{
	Value a = 5, b = 10.4;
	Value c = executeOperation(Operation::Modulo, a, b);

	std::cout << std::get<double>(c) << '\n';

	return 0;
}
