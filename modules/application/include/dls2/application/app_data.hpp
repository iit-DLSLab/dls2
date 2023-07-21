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

#include <boost/process.hpp>

/// A struct representing the app process
namespace dls
{
	class AppData
    {
    public:
        AppData(const std::string&);
        ~AppData();

        std::string getID();

        std::shared_ptr<boost::process::child> proc;

    private:
        std::string ID_;
    };
}// end namespace dls

#endif /* end of include guard: APP_DATA_HPP */
