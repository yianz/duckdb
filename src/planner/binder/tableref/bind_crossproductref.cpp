#include "duckdb/parser/tableref/crossproductref.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/tableref/bound_crossproductref.hpp"

namespace duckdb {
using namespace std;

unique_ptr<BoundTableRef> Binder::Bind(CrossProductRef &ref) {
	auto result = make_unique<BoundCrossProductRef>();
	result->left_binder = make_unique<Binder>(context, this);
	result->right_binder = make_unique<Binder>(context, this);
	auto &left_binder = *result->left_binder;
	auto &right_binder = *result->right_binder;

	result->left = left_binder.Bind(*ref.left);
	result->right = right_binder.Bind(*ref.right);

	bind_context.AddContext(move(left_binder.bind_context));
	bind_context.AddContext(move(right_binder.bind_context));
	MoveCorrelatedExpressions(left_binder);
	MoveCorrelatedExpressions(right_binder);
	return move(result);
}

} // namespace duckdb
