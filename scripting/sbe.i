%module(naturalvar=1,directors=1) SBE

%include stl.i
%include std_string.i
%naturalvar std::string;
using std;
using std::string;

%{
#include "sbe/event/Event.hpp"
#include "sbe/event/EventUser.hpp"
#include "sbe/Module.hpp"
%}

%ignore sbe::EventUser::RegisterForEvent( const Event::EventType&, int);

%include "sbe/event/Event.hpp"
%include "sbe/event/EventUser.hpp"
%include "sbe/Module.hpp"

