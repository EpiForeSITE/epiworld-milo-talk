#include <cstring>
#include <stdexcept>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

#include <pybind11/iostream.h>
#include <pybind11/numpy.h> // for py::array_t
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // silently fails when removed.

#include "database.hpp"
#include "epiworld-common.hpp"
#include "misc.hpp"
#include "model-bones.hpp"
#include "model.hpp"
#include "saver.hpp"

namespace py = pybind11;
using namespace epiworld;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

PYBIND11_MODULE(_core, m) {
	/* m.doc() = R"pbdoc(
        Epiworld Python Wrapper
        -----------------------

        .. currentmodule:: epiworldpy

        .. autosummary::
           :toctree: _generate

           ModelSEIR
    )pbdoc"; */

	auto model = py::class_<Model<int>>(
		m, "Model", "A generic model of some kind; a parent class.");
	auto database = py::class_<DataBase<int>, std::shared_ptr<DataBase<int>>>(
		m, "DataBase", "A container for data generated by a model run.");
	auto saver =
		py::class_<epiworldpy::Saver, std::shared_ptr<epiworldpy::Saver>>(
			m, "Saver", "Saves the result of multiple runs.");

	epiworldpy::export_model(model);
	epiworldpy::export_all_models(m);
	epiworldpy::export_database(database);
	epiworldpy::export_saver(saver);

#ifdef VERSION_INFO
	/* Give the real version. */
	m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
	/* Also give the real version, but prefix with 'dev'. */
	m.attr("__version__") = "dev-" MACRO_STRINGIFY(VERSION_INFO);
#endif
}
