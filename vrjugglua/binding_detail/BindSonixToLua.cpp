/**	@file
        @brief	implementation of Sonix binding

        @date
        2009-2011

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2009-2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Local includes
#include "BindSonixToLua.h"
#include "../VRJLuaOutput.h"
#include "../osgLuaBind.h"

// Library/third-party includes
#include <luabind/luabind.hpp>
#include <osg/Vec4>
#include <osg/Vec3d>
#include <snx/SoundHandle.h>

// Standard includes
// - none

namespace {
    osg::Vec3f scalingGetPosition(const snx::SoundHandle &h) {
        const double feetToMeters = 0.3048;
        float x, y, z;
        snx::SoundHandle &hbetter = const_cast<snx::SoundHandle &>(h);
        hbetter.getPosition(x, y, z);
        return osg::Vec3f(x, y, z) * feetToMeters;
    }
    void scalingSetPosition(snx::SoundHandle &h, osg::Vec3d const &vec) {
        const double metersToFeet = 3.28084;
        osg::Vec3d scaledVec = vec * metersToFeet;
        h.setPosition(scaledVec.x(), scaledVec.y(), scaledVec.z());
        return;
    }
}

namespace vrjLua {
    using namespace luabind;

    namespace Sonix {
        void changeAPI(const std::string &api) {
            snx::sonix::instance()->changeAPI(api);
        }
    } // end of Internal namespace

    void bindSonixToLua(lua_State *L) {
        module(L, "snx")
            [def("changeAPI", &Sonix::changeAPI),
             class_<snx::SoundInfo>("SoundInfo")
                 .def(constructor<>())
                 .def_readwrite("alias", &snx::SoundInfo::alias)
                 .def_readwrite("filename", &snx::SoundInfo::filename)
                 .def_readwrite("ambient", &snx::SoundInfo::ambient)
                 .def_readwrite("repeat", &snx::SoundInfo::repeat)
                 .def_readwrite("volume", &snx::SoundInfo::volume)
                 .def_readwrite("streaming", &snx::SoundInfo::streaming),
             class_<snx::SoundHandle>("SoundHandle")
                 .def(constructor<const std::string &>())
                 .def("configure", &snx::SoundHandle::configure)
                 .def("remove", &snx::SoundHandle::remove)
                 .property("name", &snx::SoundHandle::getName)
                 .def("trigger", &snx::SoundHandle::trigger)
                 .property("isPlaying", &snx::SoundHandle::isPlaying)
                 .def("stop", &snx::SoundHandle::stop)
                 .def("pause", &snx::SoundHandle::pause)
                 .def("unpause", &snx::SoundHandle::unpause)
                 .property("isPaused", &snx::SoundHandle::isPaused)
				 .def("setVolume",&snx::SoundHandle::setVolume)
				 .def("setPitchBend",&snx::SoundHandle::setPitchBend)
                 .property("ambient", &snx::SoundHandle::isAmbient,
                           &snx::SoundHandle::setAmbient)
                 .property("retriggerable", &snx::SoundHandle::isRetriggerable,
                           &snx::SoundHandle::setRetriggerable)
                 .property("position", &scalingGetPosition,
                           &scalingSetPosition)];
    }

} // end of vrjLua namespace
