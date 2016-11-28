#include "androidhelper.h"

#include <QAndroidJniObject>
#include <QtAndroid>

AndroidHelper::AndroidHelper()
{

}

void AndroidHelper::setScreenOrientation(int orientation)
{
QAndroidJniObject activity = QtAndroid::androidActivity();
if ( activity.isValid() )
    {
        activity.callMethod<void>
                ("setRequestedOrientation" // method name
                 , "(I)V" // signature
                 , orientation);
    }
}

