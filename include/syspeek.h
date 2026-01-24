//syspeek.h
//Declarations for syspeek configuration
#pragma once

struct SyspeekConfig
{
    int follow_forks ;
    int capture_args ;
    int capture_return ;
    int capture_error ;
    const char* arch ;

};