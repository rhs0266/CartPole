#include "simulation.h"
#include <iostream>


Simulation::
Simulation()
{

}


boost::python::list
Simulation::
Print()
{
    double v[4] = {1,2,3,65};
    boost::python::list vec;
    vec.append(v[0]);
    vec.append(v[1]);
    vec.append(v[2]);
    vec.append(v[3]);
    return vec;
}

using namespace boost::python;

BOOST_PYTHON_MODULE(cp)
{
    class_<Simulation>("Simulation",init<>())
            .def("Print",&Simulation::Print);
}


