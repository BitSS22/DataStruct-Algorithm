#pragma once

template <typename Type>
concept _has_graph_interface = requires(Type _T)
{
	_T.Func();
};

template <typename Type>
concept _has_graph_data = requires(Type _T)
{
	_T.ValidDest();
	_T.ValidDest(static_cast<size_t>(0));
	_T.ValidWeight();
};
