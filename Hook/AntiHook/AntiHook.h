#ifndef _AntiHook_H_
#define _AntiHook_H_

#ifdef BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

DLLIMPORT void ActivateAntiHook(int activate);

#endif