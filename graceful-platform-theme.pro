TEMPLATE            = subdirs

SUBDIRS             += \
    style                               \
    demo                                \


OTHER_FILES         += \
    $$PWD/PKGBUILD                      \



include($$PWD/icon/graceful-icon.pri)
include($$PWD/theme/graceful-theme.pri)
include($$PWD/terminator/graceful-terminator.pri)


equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 12) {
    message("Cannot use Qt $${QT_VERSION}")
    error("Qt 5.12 and newer is required")
}
