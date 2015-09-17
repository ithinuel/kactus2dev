//-----------------------------------------------------------------------------
// File: BuildCommand.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 15.09.2015
//
// Description:
// Describes the ipxact:buildCommand element.
//-----------------------------------------------------------------------------

#include "BuildCommand.h"

/*
BuildCommand::BuildCommand(QDomNode &buildCommandNode):
BuildModel(buildCommandNode), targetName_(QString()),
targetNameAttributes_() {

	for (int i = 0; i < buildCommandNode.childNodes().count(); ++i) {
		QDomNode tempNode = buildCommandNode.childNodes().at(i);

		if (tempNode.nodeName() == QString("spirit:targetName")) {
			targetName_ = tempNode.childNodes().at(0).nodeValue();
			targetNameAttributes_ = XmlUtils::parseAttributes(tempNode);
		}
	}
	return;
}*/

//-----------------------------------------------------------------------------
// Function: BuildCommand::BuildCommand()
//-----------------------------------------------------------------------------
BuildCommand::BuildCommand() :
BuildModel(),
targetName_(),
flagsAppend_()
{

}

//-----------------------------------------------------------------------------
// Function: BuildCommand::BuildCommand()
//-----------------------------------------------------------------------------
BuildCommand::BuildCommand(const BuildCommand &other) :
BuildModel(other),
targetName_(other.targetName_),
flagsAppend_(other.flagsAppend_)
{

}

//-----------------------------------------------------------------------------
// Function: BuildCommand::BuildCommand()
//-----------------------------------------------------------------------------
BuildCommand & BuildCommand::operator=(const BuildCommand &other)
{
    if (this != &other)
    {
        BuildModel::operator =(other);
        targetName_ = other.targetName_;
        flagsAppend_ = other.flagsAppend_;
    }

    return *this;
}

//-----------------------------------------------------------------------------
// Function: BuildCommand::~BuildCommand()
//-----------------------------------------------------------------------------
BuildCommand::~BuildCommand()
{

}

//-----------------------------------------------------------------------------
// Function: BuildCommand::getTargetName()
//-----------------------------------------------------------------------------
QString BuildCommand::getTargetName() const
{
    return targetName_;
}

//-----------------------------------------------------------------------------
// Function: BuildCommand::setTargetName()
//-----------------------------------------------------------------------------
void BuildCommand::setTargetName(const QString &targetName)
{
    targetName_ = targetName;
}

//-----------------------------------------------------------------------------
// Function: BuildCommand::isFlagsAppend()
//-----------------------------------------------------------------------------
QString BuildCommand::getFlagsAppend() const
{
    return flagsAppend_.toString();
}

//-----------------------------------------------------------------------------
// Function: BuildCommand::setFlagsAppend()
//-----------------------------------------------------------------------------
void BuildCommand::setFlagsAppend(bool newAppend)
{
    flagsAppend_.setValue(newAppend);
}

//-----------------------------------------------------------------------------
// Function: BuildCommand::clearFlagsAppend()
//-----------------------------------------------------------------------------
void BuildCommand::clearFlagsAppend()
{
    flagsAppend_.setUnspecified();
}
