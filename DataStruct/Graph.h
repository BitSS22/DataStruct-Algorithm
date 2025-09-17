#pragma once

template <typename Type>
concept _has_graph_interface = requires(Type _T)
{
	_T.Func();
};

template <typename Type>
concept _has_graph_data = requires(Type _T)
{
	_T.GetData();
};
