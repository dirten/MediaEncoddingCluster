/* 
 * File:   MainMenuActionHandler.h
 * Author: jhoelscher
 *
 * Created on 31. August 2009, 14:15
 */

#ifndef _MAINMENUACTIONHANDLER_H
#define	_MAINMENUACTIONHANDLER_H

class MainMenuActionHandler{
public:
    void listAllFiles();
    void listImportedFiles();
    void listEncodedFiles();

    void listAllEncodings();
    void listPendingEncodings();
    void listActiveEncodings();
    void listSuccessfullEncodings();
    void listFailureEncodings();

    void listAllProfiles();
    void createProfiles();

    void listAllWatchfolder();
    void createWatchfolder();

    void editSystemConfiguration();

};

#endif	/* _MAINMENUACTIONHANDLER_H */

