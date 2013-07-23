#include "ExpandableMatrix.h"




ExpandableMatrixPair::ExpandableMatrixPair(size_t matrixCount, 
										   size_t size, 
										   size_t horzRowCount, 
										   size_t vertColCount,
										   double dt)
{
	_size = size;
	_horzRowCount = horzRowCount;
	_vertColCount = vertColCount;
	_dt = dt;

	_totalSize = matrixCount * _size;

	_horz = matrix< double>(_horzRowCount, _totalSize);
	_vert = matrix< double>(matrixCount * _size, _vertColCount);

	_horzIndex = 0;
	_vertIndex = _totalSize - _size;

	_horzrange = range (0, _horzRowCount);
	_vertrange = range (0, _vertColCount);

}

void ExpandableMatrixPair::Add(matrix< double>& horz, matrix< double>& vert, matrix<double>& out)
{
	project(_horz, _horzrange, range (_horzIndex, _horzIndex + _size)).assign(horz);
	project(_vert, range (_vertIndex, _vertIndex + _size), _vertrange).assign(vert);

	_horzIndex += _size;

	out.assign(_dt * prod(
		project(_horz, _horzrange, range (0, _horzIndex)),
		project(_vert, range (_vertIndex, _totalSize ), _vertrange)));

	_vertIndex -= _size;

}
