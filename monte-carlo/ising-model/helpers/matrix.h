#ifndef GDS_MATRIX_H_INCLUDED
#define GDS_MATRIX_H_INCLUDED


//////////////////////////////////////////////////////////////////////////
//
// gds::matrix<T>
//
// Simple class to store 2D matrix.
//
// Written by Giovanni Dicanio <gdicanio@mvps.org>
//
// 2010, December 28th
// Last update: 2011, January 8th
//
//
// Index Bounds Checking
// ---------------------
// In debug builds, indexes are bounds checked (exceptions are thrown
// if indexes are out of bound).
// No index checking is done in release builds, for performance reasons.
//
//
// NOTE:
//
// For more advanced matrix stuff see libraries like Blitz++:
//
//  http://www.oonumerics.org/blitz/
//
//
//////////////////////////////////////////////////////////////////////////



#include <stdexcept>    // STL exceptions
#include <vector>       // std::vector used to store matrix elements



namespace gds {


//------------------------------------------------------------------------
// Simple 2D matrix template class.
//------------------------------------------------------------------------
template <typename T>
class matrix
{
public:

    // Creates an empty matrix.
    matrix();

    // Creates a matrix with given number of rows and columns.
    // If both row and column count is 0, the matrix is set to an empty matrix.
    matrix(size_t rows, size_t columns);
    
    // Is this an empty matrix?
    bool empty() const;

    // Number of rows
    int rows() const;

    // Number of columns
    int columns() const;

    // Read-only access to the matrix element at given position 
    // (row and column indexes are 0-based).
    // In debug builds, throws exception in case of index out of range.
    // No index checking is done in release builds for performance reasons.
    const T & operator()(size_t row, size_t col) const;

    // Writable access to the matrix element at given position 
    // (row and column indexes are 0-based).
    // In debug builds, throws exception in case of index out of range.
    // No index checking is done in release builds for performance reasons.
    T & operator()(size_t row, size_t col);   
    // Resizes a matrix with given number of rows and columns.
    // Do not assume that previous matrix data is preserved.
    // If both row and column count is 0, the matrix is set to an empty matrix.
    void resize(size_t rows, size_t columns);
  
    // Resets to an empty matrix
    // (does nothing if the matrix is already empty).
    void clear();


    //
    // IMPLEMENTATION
    //
private:
    std::vector<T> m_data;      // matrix data, stored row-wise
    size_t m_rows;              // row count
    size_t m_cols;              // column count


    // Given a row and column 2D index, builds the corresponding 1D index
    // in the 1D array containing matrix elements.
    // In debug builds throws an exception if index is out of range.
    int linearize_index(size_t row, size_t col) const;
};



//------------------------------------------------------------------------
//                      METHOD IMPLEMENTATIONS
//------------------------------------------------------------------------


template <typename T>
inline matrix<T>::matrix()
    : m_rows(0)
    , m_cols(0)
{
}


template <typename T>
inline matrix<T>::matrix(size_t rows, size_t columns)
    : m_rows(0)
    , m_cols(0)
{
    // Check special case of empty matrix
    if (rows == 0 && columns == 0)
    {
        // Empty matrix - nothing more to do
        return;
    }

    // Having one dimension 0 and the other one non-0 is an error
    if (rows == 0)
        throw std::invalid_argument("[gds::matrix<T>] Invalid row count in constructor.");
    if (columns == 0)
        throw std::invalid_argument("[gds::matrix<T>] Invalid column count in constructor.");

    // Resize vector according to matrix dimension
    m_data.resize(rows * columns);

    // Store matrix dimension
    m_rows = rows;
    m_cols = columns;
}


template <typename T>
inline bool matrix<T>::empty() const
{
    return (m_rows == 0 && m_cols == 0);
}


template <typename T>
inline int matrix<T>::rows() const
{
    return m_rows;
}


template <typename T>
inline int matrix<T>::columns() const
{
    return m_cols;
}


template <typename T>
inline const T & matrix<T>::operator()(size_t row, size_t col) const
{
    // Check row and column index in debug builds
#ifdef _DEBUG
    if (row >= m_rows)
        throw std::invalid_argument("[gds::matrix<T>] Row index out of bound.");

    if (col >= m_cols)
        throw std::invalid_argument("[gds::matrix<T>] Column index out of bound.");
#endif // _DEBUG

    return m_data[linearize_index(row, col)];
}


template <typename T>
inline T & matrix<T>::operator()(size_t row, size_t col)
{
    // Check row and column index in debug builds
#ifdef _DEBUG
    if (row >= m_rows)
        throw std::invalid_argument("[gds::matrix<T>] Row index out of bound.");

    if (col >= m_cols)
        throw std::invalid_argument("[gds::matrix<T>] Column index out of bound.");
#endif // _DEBUG

    return m_data[linearize_index(row, col)];
}


template <typename T>
inline void matrix<T>::resize(size_t rows, size_t columns)
{
    // Special case of empty matrix
    if (rows == 0 && columns == 0)
    {
        // Set as empty matrix
        clear();
        return;
    }

    // Special case of useless resizing
    if (rows == m_rows && columns == m_cols)
        return; // Nothing to do (desidered size == current size)

    // Having one dimension 0 and the other one non-0 is an error
    if (rows == 0)
        throw std::invalid_argument("[gds::matrix<T>] Invalid row count in resize() method.");
    if (columns == 0)
        throw std::invalid_argument("[gds::matrix<T>] Invalid column count in resize() method.");

    // Resize element vector
    m_data.resize(rows*columns);

    // Update matrix size data members
    m_rows = rows;
    m_cols = columns;
}


template <typename T>
inline void matrix<T>::clear()
{
    if (empty())
    {
        // Nothing to do: the matrix is already empty.
        return;
    }

    // Clear row and column count
    m_rows = m_cols = 0;

    // Clear element vector
    m_data.clear();
}


template <typename T>
inline int matrix<T>::linearize_index(size_t row, size_t col) const
{
    // Check row and column index in debug builds
#ifdef _DEBUG
    if (row >= m_rows)
        throw std::invalid_argument("[gds::matrix<T>] Row index out of bound.");

    if (col >= m_cols)
        throw std::invalid_argument("[gds::matrix<T>] Column index out of bound.");
#endif // _DEBUG


    // Matrix elements are stored row-wise
    return col + row*m_cols;
}



} // namespace gds


#endif // GDS_MATRIX_H_INCLUDED
