%module(directors=1) SBE

%include stl.i
%naturalvar;
using std;

%{
#include "sbe/event/Event.hpp"
#include "sbe/event/EventUser.hpp"
#include "sbe/Module.hpp"
%}

%include "sbe/event/Event.hpp"
%include "sbe/event/EventUser.hpp"
%include "sbe/Module.hpp"

