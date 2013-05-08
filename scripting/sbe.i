%module SBE

%include stl.i
%include std_string.i
%naturalvar;
using std;
using std::string;

%{
#include "sbe/event/Event.hpp"
#include "sbe/event/EventUser.hpp"
#include "sbe/Module.hpp"
%}


%include "sbe/event/Event.hpp"
%include "sbe/event/EventUser.hpp"
%include "sbe/Module.hpp"

