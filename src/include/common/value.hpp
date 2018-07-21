
#pragma once

#include <memory.h>

#include "common/internal_types.hpp"
#include "common/printable.hpp"

namespace duckdb {
class Value : public Printable {
  public:
	Value() : type(TypeId::INTEGER), is_null(true) {}
	Value(int32_t val) : type(TypeId::INTEGER), is_null(false) {
		value_.integer = val;
	}
	Value(double val) : type(TypeId::INTEGER), is_null(false) {
		value_.decimal = val;
	}
	Value(std::string val) : type(TypeId::VARCHAR), is_null(false) {
		value_.data = new char[val.size() + 1];
		memcpy(value_.data, val.c_str(), val.size());
		value_.data[val.size()] = '\0';
		len = val.size();
	}
	Value(const Value &other);
	~Value() {
		if ((type == TypeId::VARCHAR || type == TypeId::VARBINARY ||
		     type == TypeId::ARRAY) &&
		    value_.data) {
			delete[] value_.data;
		}
	}

	Value Copy() { return Value(*this); }

	virtual std::string ToString() const;
	
	Value CastAs(TypeId new_type);

//  private:
	TypeId type;
	bool is_null;

	union Val {
		int8_t boolean;
		int8_t tinyint;
		int16_t smallint;
		int32_t integer;
		int64_t bigint;
		double decimal;
		int32_t date;
		uint64_t timestamp;
		char *data;
	} value_;

	uint32_t len;
};
}