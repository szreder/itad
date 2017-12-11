#include <cinttypes>
#include <cstring>
#include <string>

struct Value {
	enum class Type : uint8_t {
		Boolean,
		Integer,
		Real,
		String,
	} type;

	union {
		bool b;
		int i;
		double d;
		char *s; //std::string is non-trivial
	};
};

enum class Operation : uint8_t {
	Plus,
	Minus,
	Multiply,
	Divide,
	Modulo,
};

Value executeOperation(Operation op, const Value &left, const Value &right)
{
	Value result;

	if (left.type != right.type) {
		if ((left.type == Value::Type::Integer && right.type == Value::Type::Real)
		    || (left.type == Value::Type::Real && right.type == Value::Type::Integer))
			result.type = Value::Type::Real;
		else
			abort();
	} else {
		result.type = left.type;
	}

	switch (result.type) {
		case Value::Type::Integer:
			switch (op) {
				case Operation::Plus:
					result.i = left.i + right.i;
					break;
				case Operation::Minus:
					result.i = left.i - right.i;
					break;
				//etc
			}
			break;

		case Value::Type::String:
			switch (op) {
				case Operation::Plus:
					result.s = (char *)malloc(strlen(left.s) + strlen(right.s) + 1);
					//copy strings
					break;
				default:
					abort(); //unhandled type for this operations
			}
			break;
	}

	return result;
}

int main()
{
	return 0;
}
