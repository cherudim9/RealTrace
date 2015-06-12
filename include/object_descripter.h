#ifndef _OBJECT_DESCRIPTER_H
#define _OBJECT_DESCRIPTER_H

#pragma once
#include <cassert>
#include "basic_geometry.h"
#include <vector>
#include <cstring>

namespace ObjectDescripter
{
    template <typename T, int N> class Array 
    {
    public:
      enum {_len = N};
      typedef T t_Val; 
    public:
      T& operator[] (int i)
        {
	  assert(i>=0&&i<N);
	  return _p[i];
        }
      const T& operator[] (int i) const 
      {
	assert(i>=0&&i<N);
	return _p[i];
      }

    protected:
      T _p[N];
    };

    class ObjectT
    {
    public:
        ObjectT(void);
        ~ObjectT(void);
        
    public:
        bool IsLoaded() { return m_pVertexList!=NULL;}

        void Destroy();
        bool LoadFromObj(const char* fn);
        bool SaveToObj(const char* fn);

	void Init();
        
    protected:
        bool Parse(FILE* fp);
        bool CheckParse(int nVertices,std::vector<Array<int,3> > & vecTriangles);

    public:

        int             m_nVertices;
        int             m_nTriangles;
        PointT*          m_pVertexList;
        Array<int,3>*   m_pTriangleList;
	PointT* triangle_normal_, *vertex_normal_;
	int *vertex_triangle_relation_number_;
	int **vertex_triangle_relation_;

    };

}

#endif
