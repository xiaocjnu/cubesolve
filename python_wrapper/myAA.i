%module mycube


%begin %{
#define SWIG_PYTHON_INTERPRETER_NO_DEBUG
%}

//%include "stl.i"
%include "std_vector.i"
%include "std_string.i"
//%include "typemaps.i"
%include "std_array.i"
%include "std_map.i"
%include "carrays.i"


%{
#include "cs.h"
%}



//%template(CharVector) std::vector<char>;
%template(UCharVector) std::vector<unsigned char>;

%template(IntArray54) std::array<int,54>;
%template(DoubleArray6) std::array<double, 6>;

%template(IntVector) std::vector<int>;
%template(DoubleVector) std::vector<double>;


%template(eoArray12) std::array<cs::eo_t,12>;
%template(coArray8) std::array<cs::co_t,8>;


//%array_functions(int, IntCArray)
//%array_functions(float, FloatCArray)

%array_class(int, IntCArray)
%array_class(float, FloatCArray)


%rename (_mul) operator*;
%rename (equals) operator==;
%rename (assignFrom) operator=;
%rename (plus) operator+;
%rename (minus) operator-;
%rename (_toString) toString;


typedef unsigned int GLuint;

%{
typedef unsigned int GLuint;
%}





%include "util.h"

%include "std_array.i"

%template(MoveVector) std::vector<cs::Move>;

%template(IntArray6) std::array<int,6>;
%template(IntArray8) std::array<int,8>;

%template(Permutation6) cs::permutation<6>;
%template(Permutation8) cs::permutation<8>;

%template(SingleFaceColor) std::array<int, 9>;
%template(SixFacesColor) std::array<std::array<int, 9>, 6>;


%template(CornerType) cs::permutation_orientation_t<cs::corner_orientation, 8>;
%template(EdgeType) cs::permutation_orientation_t<cs::edge_orientation, 12>;

//%rename(myc_Coordinate) my::Coordinate;
//%rename(myc_matrix) my::matrix;
//%rename(myc_quaternion) my::quaternion;
//%rename(myc_vec3) my::vec3;


//**************************************
%ignore cs::CubieTables;
%ignore cs::CoordCubeTables;
%ignore cs::CoordCubeTables5;

%ignore cs::CubieCube::tables;

%ignore cs::CoordCube;
%ignore cs::CoordCube5;

/*
%immutable my::CubeScene::cube;
%immutable my::CubeScene::modelgroups;

%immutable my::CubeControl::cubeScene;
%immutable my::CubeControl::sceneHandler;

%immutable my::CubeControlNew::cubeScene;
%immutable my::CubeControlNew::sceneHandler;

%immutable my::CubeSceneHandler::myCubeScene;
%immutable my::CubeSceneHandler::myCube;
%immutable my::CubeSceneHandler::myCam;
%immutable my::CubeSceneHandler::myCubeAni;
%immutable my::CubeSceneHandler::myCoordAni;
%immutable my::CubeSceneHandler::myArrowAni;
*/
//**************************************

%include "util.h"
%include "AxisCube.h"
%include "FaceCubeBase.h"
%include "CubieCube.h"
%include "AxisCubieCube.h"
%include "HumanSolver.h"
%include "CoordCube.h"
%include "CoordCube5.h"
%include "Search.h"
%include "Search5.h"


%rename(cs_init) cs::init;
%rename (cs_init_without_search) cs::init_without_search;

%include "cs.h"

//********************************************



%{
#include "tools.h"
%}

%include "tools_typedef.h"
%include "good_tools.h"
%include "tools.h"
%include "CfopAnalysis.h"



//%typemap(in, noblock=1) char {$1= (char)PyInt_AsLong($input);}
//%typemap(out, noblock=1) char {$result = PyInt_FromLong($1);}

%include CubeFrame.i






//********************************************

%template(CFRecordVector) std::vector<cf_record_t>;
%template(CFRichRecordVector) std::vector<cf_rich_record_t>;
