/*
 Oolong Engine for the iPhone / iPod touch
 Copyright (c) 2007-2008 Wolfgang Engel  http://code.google.com/p/oolongengine/
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose, 
 including commercial applications, and to alter it and redistribute it freely, 
 subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
*/

/*
 *  ResourcePath.cpp
 *
 *  Created by astojilj@gmail.com on 7/12/11 and added to oolongengine.
 *
 */

#ifdef QT_BUILD
#include <QCoreApplication>
#include <QString>
#include <QDebug>


//! Change for MeeGo - form the path as /usr/share/<applicationname>
void GetResourcePathASCII(char* cBuffer, int iLength)
{
    const QString applicationExecutableName = QCoreApplication::applicationFilePath().right(QCoreApplication::applicationFilePath().length() - QCoreApplication::applicationFilePath().lastIndexOf("/") -1);
    const QString path = QCoreApplication::applicationDirPath() + QString::fromLatin1("/../share/%1").arg(applicationExecutableName);
    if (path.length() <= iLength)
        strcpy(cBuffer,path.toAscii().constData());
}
#endif
