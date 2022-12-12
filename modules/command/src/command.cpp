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
#ifndef COMMAND_CPP
#define COMMAND_CPP

#include "dls2/command/command.hpp"
#include "dls2/command/command_manager.hpp"

namespace dls
{
    template<> double transform_args<double>(std::string const &s) { return atof(s.c_str());}
    template<> int transform_args<int>(std::string const &s) { return atoi(s.c_str());}
    template<> bool transform_args<bool>(std::string const &s) { return (s != "0");}
    template<> std::string transform_args<std::string>(std::string const &s) { return s;}


    void CommandHelper::changeCommandLevel(CommandManager *manager, int level)
    {
        manager->changeLevel(level);
    }

    int CommandHelper::getCurrentLevel(CommandManager *manager)
    {
        return manager->getCurrentLevel();
    }
} // end namespace dls

#endif /* end of include guard: COMMAND_CPP */