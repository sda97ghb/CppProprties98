TEMPLATE = subdirs

SUBDIRS += \
    Library \
    Tests
    
Tests.depends = Library
