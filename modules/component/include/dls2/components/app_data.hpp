/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef APP_DATA_HPP
#define APP_DATA_HPP

#include "dls2/util/messaging/dds_reader.hpp"
#include <boost/process.hpp>

#include <memory>

/// A struct representing the control signal that is output by a Controller
namespace dls
{
	class AppData
    {
    public:
        AppData();
        ~AppData();

        std::shared_ptr<boost::process::child> proc;
        std::shared_ptr<DDSReader> dds_reader;
        std::string ID;
    };
}// end namespace dls

#endif /* end of include guard: APP_DATA_HPP */
