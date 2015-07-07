android-no-sdk {
    target.path = /data/user/qt
    export(target.path)
    INSTALLS += target
} else:android {
    x86 {
        target.path = /libs/x86
    } else: armeabi-v7a {
        target.path = /libs/armeabi-v7a
    } else {
        target.path = /libs/armeabi
    }
    export(target.path)
    INSTALLS += target
} else:unix {          # Esta es configuración específica de Linux y UNIX
    # Variables
    #
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    BINDIR  = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    CONFDIR = /etc
    isEmpty(VARDIR) {
        VARDIR  = /var/lib/$${TARGET}
    }
    DEFINES += APP_DATADIR="$$DATADIR"
    DEFINES += APP_VARDIR="$$VARDIR"
    DEFINES += APP_CONFFILE="$$CONFDIR/$${TARGET}.ini"
    # Install
    #
    ## Instalar ejecutable
    target.path = $$BINDIR
    ## Instalar archivo de configuración
    config.path = $$CONFDIR
    config.files += $${TARGET}.ini
    ## Instalar acceso directo en el menú del escritorio
    desktop.path = $$DATADIR/applications
    desktop.files += $${TARGET}.desktop
    ## Instalar icono de aplicación
    icon32.path = $$DATADIR/icons/hicolor/32x32/apps
    icon32.files += ./data/32x32/$${TARGET}.png
    ## Crear directorio de archivos variables
    vardir.path = $$VARDIR
    vardir.commands = true
    INSTALLS += target config desktop icon32 vardir
}
#} else:unix {
#    isEmpty(target.path) {
#        qnx {
#            target.path = /tmp/$${TARGET}/bin
#        } else {
#            target.path = /opt/$${TARGET}/bin
#        }
#        export(target.path)
#    }
#    INSTALLS += target
#}

export(INSTALLS)
