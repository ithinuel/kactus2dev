#-----------------------------------------------------------------------------
# File: subdirs.pro
#-----------------------------------------------------------------------------
# Project: Kactus2
# Author: Esko Pekkarinen
# Date: 24.7.2014
#
# Description:
# Qt project file template for running unit tests in subdirectories.
#-----------------------------------------------------------------------------

TEMPLATE = subdirs

CONFIG += testcase
SUBDIRS += <subdir/subproject.pro>
