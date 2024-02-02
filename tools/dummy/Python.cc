#include <fstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace pybind11::literals;

class Dummy
{
public:
  Dummy(){}
  virtual ~Dummy() = default;
  std::string call() const {
    return "dummy";
  }
};

PYBIND11_MODULE(_ext, m)
{
  py::class_<Dummy>(m, "Dummy")
    .def(py::init())
    .def("__call__", &Dummy::call);
}
