//-----------------------------------------------------------------------------
// File: ComponentReader.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 14.10.2015
//
// Description:
// Reader class for ipxact:component element.
//-----------------------------------------------------------------------------

#include "ComponentReader.h"
#include "Component.h"

#include <IPXACTmodels/common/NameGroupReader.h>
#include <IPXACTmodels/Component/BusInterfaceReader.h>
#include <IPXACTmodels/Component/ChannelReader.h>
#include <IPXACTmodels/Component/RemapStateReader.h>
#include <IPXACTmodels/Component/AddressSpaceReader.h>
#include <IPXACTmodels/Component/MemoryMapReader.h>
#include <IPXACTmodels/Component/ViewReader.h>
#include <IPXACTmodels/Component/InstantiationsReader.h>
#include <IPXACTmodels/Component/PortReader.h>
#include <IPXACTmodels/Component/ComponentGeneratorReader.h>
#include <IPXACTmodels/Component/ChoiceReader.h>
#include <IPXACTmodels/Component/FileSetReader.h>
#include <IPXACTmodels/Component/CPUReader.h>
#include <IPXACTmodels/Component/OtherClockDriverReader.h>
#include <IPXACTmodels/Component/IndirectInterface.h>
#include <IPXACTmodels/Component/IndirectInterfaceReader.h>

#include <IPXACTmodels/kactusExtensions/ComProperty.h>
#include <IPXACTmodels/kactusExtensions/SystemView.h>
#include <IPXACTmodels/kactusExtensions/ComInterface.h>
#include <IPXACTmodels/kactusExtensions/ApiInterface.h>
#include <IPXACTmodels/kactusExtensions/FileDependency.h>

#include <IPXACTmodels/Component/Model.h>

//-----------------------------------------------------------------------------
// Function: ComponentReader::ComponentReader()
//-----------------------------------------------------------------------------
ComponentReader::ComponentReader(): DocumentReader()
{

}

//-----------------------------------------------------------------------------
// Function: ComponentReader::~DocumentReader()
//-----------------------------------------------------------------------------
ComponentReader::~ComponentReader()
{

}

//-----------------------------------------------------------------------------
// Function: ComponentReader::createComponentFrom()
//-----------------------------------------------------------------------------
QSharedPointer<Component> ComponentReader::createComponentFrom(QDomDocument const& componentDocument) const
{
    QSharedPointer<Component> newComponent (new Component());

    parseTopComments(componentDocument, newComponent);

    parseXMLProcessingInstructions(componentDocument, newComponent);

    QDomNode componentNode = componentDocument.firstChildElement();
    parseNamespaceDeclarations(componentNode, newComponent);

    parseVLNVElements(componentNode, newComponent, VLNV::COMPONENT);

    parseBusInterfaces(componentNode, newComponent);

    parseIndirectInterfaces(componentNode, newComponent);

    parseChannels(componentNode, newComponent);

    parseRemapStates(componentNode, newComponent);

    parseAddressSpaces(componentNode, newComponent);

    parseMemoryMaps(componentNode, newComponent);

    parseModel(componentNode, newComponent);

    parseComponentGenerators(componentNode, newComponent);

    parseChoices(componentNode, newComponent);

    parseFileSets(componentNode, newComponent);

    parseCPUs(componentNode, newComponent);

    parseOtherClockDrivers(componentNode, newComponent);

    parseDescription(componentNode, newComponent);

    parseParameters(componentNode, newComponent);

    parseAssertions(componentNode, newComponent);

    parseComponentExtensions(componentNode, newComponent);

    return newComponent;
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseBusInterfaces()
//-----------------------------------------------------------------------------
void ComponentReader::parseBusInterfaces(QDomNode const& componentNode, QSharedPointer<Component> newComponent)
    const
{
    QDomElement busInterfaceElement = componentNode.firstChildElement(QStringLiteral("ipxact:busInterfaces"));

    if (!busInterfaceElement.isNull())
    {
        BusinterfaceReader busReader;

        QDomNodeList busInterfaceNodeList = busInterfaceElement.elementsByTagName(QStringLiteral("ipxact:busInterface"));
        for (int interfaceIndex = 0; interfaceIndex < busInterfaceNodeList.count(); ++interfaceIndex)
        {
            QDomNode busNode = busInterfaceNodeList.at(interfaceIndex);
            QSharedPointer<BusInterface> newBus = busReader.createbusinterfaceFrom(busNode);

            newComponent->getBusInterfaces()->append(newBus);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseIndirectInterfaces()
//-----------------------------------------------------------------------------
void ComponentReader::parseIndirectInterfaces(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement interfaceElement = componentNode.firstChildElement(QStringLiteral("ipxact:indirectInterfaces"));

    if (!interfaceElement.isNull())
    {
        IndirectInterfaceReader interfaceReader;

        QDomNodeList interfaceList = interfaceElement.elementsByTagName(QStringLiteral("ipxact:indirectInterface"));
        for (int i = 0; i < interfaceList.count(); ++i)
        {
            QDomNode interfaceNode = interfaceList.at(i);
            QSharedPointer<IndirectInterface> newInterface = interfaceReader.createIndirectInterfaceFrom(interfaceNode);

            newComponent->getIndirectInterfaces()->append(newInterface);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseChannels()
//-----------------------------------------------------------------------------
void ComponentReader::parseChannels(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement channelsElement = componentNode.firstChildElement(QStringLiteral("ipxact:channels"));

    if (!channelsElement.isNull())
    {
        ChannelReader channelReader;

        QDomNodeList channelNodeList = channelsElement.elementsByTagName(QStringLiteral("ipxact:channel"));
        for (int channelIndex = 0; channelIndex < channelNodeList.count(); ++channelIndex)
        {
            QDomNode channelNode = channelNodeList.at(channelIndex);
            QSharedPointer<Channel> newChannel = channelReader.createChannelFrom(channelNode);

            newComponent->getChannels()->append(newChannel);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseRemapStates()
//-----------------------------------------------------------------------------
void ComponentReader::parseRemapStates(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement remapStateElement = componentNode.firstChildElement(QStringLiteral("ipxact:remapStates"));

    if (!remapStateElement.isNull())
    {
        RemapStateReader remapStateReader;

        QDomNodeList remapNodeList = remapStateElement.elementsByTagName(QStringLiteral("ipxact:remapState"));
        for (int remapStateIndex = 0; remapStateIndex < remapNodeList.count(); ++remapStateIndex)
        {
            QDomNode remapStateNode = remapNodeList.at(remapStateIndex);
            QSharedPointer<RemapState> newRemapState = remapStateReader.createRemapStateFrom(remapStateNode);

            newComponent->getRemapStates()->append(newRemapState);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseAddressSpaces()
//-----------------------------------------------------------------------------
void ComponentReader::parseAddressSpaces(QDomNode const& componentNode, QSharedPointer<Component> newComponent)
    const
{
    QDomElement addressSpacesElement = componentNode.firstChildElement(QStringLiteral("ipxact:addressSpaces"));

    if (!addressSpacesElement.isNull())
    {
        AddressSpaceReader spaceReader;

        QDomNodeList addressNodeList = addressSpacesElement.elementsByTagName(QStringLiteral("ipxact:addressSpace"));
        for (int spaceIndex = 0; spaceIndex < addressNodeList.count(); ++spaceIndex)
        {
            QDomNode addressSpaceNode = addressNodeList.at(spaceIndex);
            QSharedPointer<AddressSpace> newAddressSpace = spaceReader.createAddressSpaceFrom(addressSpaceNode);

            newComponent->getAddressSpaces()->append(newAddressSpace);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseMemoryMaps()
//-----------------------------------------------------------------------------
void ComponentReader::parseMemoryMaps(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement memoryMapsElement = componentNode.firstChildElement(QStringLiteral("ipxact:memoryMaps"));

    if (!memoryMapsElement.isNull())
    {
        MemoryMapReader memoryReader;

        QDomNodeList memoryMapNodeList = memoryMapsElement.elementsByTagName(QStringLiteral("ipxact:memoryMap"));
        for (int memoryIndex = 0; memoryIndex < memoryMapNodeList.count(); ++memoryIndex)
        {
            QDomNode memoryMapNode = memoryMapNodeList.at(memoryIndex);
            QSharedPointer<MemoryMap> newMemoryMap = memoryReader.createMemoryMapFrom(memoryMapNode);

            newComponent->getMemoryMaps()->append(newMemoryMap);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseModel()
//-----------------------------------------------------------------------------
void ComponentReader::parseModel(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement modelElement = componentNode.firstChildElement(QStringLiteral("ipxact:model"));
    if (!modelElement.isNull())
    {
        QSharedPointer<Model> newmodel (new Model());

        parseViews(modelElement, newmodel);

        parseInstantiations(modelElement, newmodel);

        parsePorts(modelElement, newmodel);

        newComponent->setModel(newmodel);
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseViews()
//-----------------------------------------------------------------------------
void ComponentReader::parseViews(QDomElement const& modelElement, QSharedPointer<Model> newModel) const
{
    QDomElement viewsElement = modelElement.firstChildElement(QStringLiteral("ipxact:views"));

    if (!viewsElement.isNull())
    {
        ViewReader viewReader;

        QDomNodeList viewNodeList = viewsElement.elementsByTagName(QStringLiteral("ipxact:view"));
        for (int viewIndex = 0; viewIndex < viewNodeList.count(); ++viewIndex)
        {
            QDomNode viewNode = viewNodeList.at(viewIndex);
            QSharedPointer<View> newView = viewReader.createViewFrom(viewNode);

            newModel->getViews()->append(newView);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseInstantiations()
//-----------------------------------------------------------------------------
void ComponentReader::parseInstantiations(QDomElement const& modelElement, QSharedPointer<Model> newModel) const
{
    QDomElement instantiationsElement = modelElement.firstChildElement(QStringLiteral("ipxact:instantiations"));
    if (!instantiationsElement.isNull())
    {
        parseComponentInstantiations(instantiationsElement, newModel);

        parseDesignInstantiations(instantiationsElement, newModel);

        parseDesignConfigurationInstantiations(instantiationsElement, newModel);
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseComponentInstantiations()
//-----------------------------------------------------------------------------
void ComponentReader::parseComponentInstantiations(QDomElement const& instantiationsElement,
    QSharedPointer<Model> newModel) const
{
    QDomNodeList componentInstantiationNodeList =
        instantiationsElement.elementsByTagName(QStringLiteral("ipxact:componentInstantiation"));
    if (!componentInstantiationNodeList.isEmpty())
    {
        InstantiationsReader instantiationsReader;

        for (int i = 0 ; i < componentInstantiationNodeList.count(); ++i)
        {
            QDomNode componentInstantiationNode = componentInstantiationNodeList.at(i);
            QSharedPointer<ComponentInstantiation> newInstantiation =
                instantiationsReader.createComponentInstantiationFrom(componentInstantiationNode);

            newModel->getComponentInstantiations()->append(newInstantiation);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseDesignInstantiations()
//-----------------------------------------------------------------------------
void ComponentReader::parseDesignInstantiations(QDomElement const& instantiationsElement,
    QSharedPointer<Model> newModel) const
{
    QDomNodeList designInstantiationNodeList =
        instantiationsElement.elementsByTagName(QStringLiteral("ipxact:designInstantiation"));
    if (!designInstantiationNodeList.isEmpty())
    {
        InstantiationsReader instantiationsReader;

        for (int i = 0 ; i < designInstantiationNodeList.count(); ++i)
        {
            QDomNode designInstantiationNode = designInstantiationNodeList.at(i);
            QSharedPointer<DesignInstantiation> newInstantiation =
                instantiationsReader.createDesignInstantiationFrom(designInstantiationNode);

            newModel->getDesignInstantiations()->append(newInstantiation);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseDesignConfigurationInstantiations()
//-----------------------------------------------------------------------------
void ComponentReader::parseDesignConfigurationInstantiations(QDomElement const& instantiationsElement,
    QSharedPointer<Model> newModel) const
{
    QDomNodeList designConfigurationInstantiationNodeList =
        instantiationsElement.elementsByTagName(QStringLiteral("ipxact:designConfigurationInstantiation"));
    if (!designConfigurationInstantiationNodeList.isEmpty())
    {
        InstantiationsReader instantiationsReader;

        for (int i = 0 ; i < designConfigurationInstantiationNodeList.count(); ++i)
        {
            QDomNode designConfigurationInstantiationNode = designConfigurationInstantiationNodeList.at(i);
            QSharedPointer<DesignConfigurationInstantiation> newInstantiation = instantiationsReader.
                createDesignConfigurationInstantiationFrom(designConfigurationInstantiationNode);

            newModel->getDesignConfigurationInstantiations()->append(newInstantiation);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parsePorts()
//-----------------------------------------------------------------------------
void ComponentReader::parsePorts(QDomElement const& modelElement, QSharedPointer<Model> newModel) const
{
    QDomElement portsElement = modelElement.firstChildElement(QStringLiteral("ipxact:ports"));

    if (!portsElement.isNull())
    {
        PortReader portReader;

        QDomNodeList portNodeList = portsElement.elementsByTagName(QStringLiteral("ipxact:port"));
        for (int portIndex = 0; portIndex < portNodeList.count(); ++portIndex)
        {
            QDomNode portNode = portNodeList.at(portIndex);
            QSharedPointer<Port> newPort = portReader.createPortFrom(portNode);

            newModel->getPorts()->append(newPort);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseComponentGenerators()
//-----------------------------------------------------------------------------
void ComponentReader::parseComponentGenerators(QDomNode const& componentNode,
    QSharedPointer<Component> newComponent) const
{
    QDomElement componentGeneratorsElement = componentNode.firstChildElement(QStringLiteral("ipxact:componentGenerators"));

    if (!componentGeneratorsElement.isNull())
    {
        ComponentGeneratorReader generatorReader;

        QDomNodeList generatorNodeList = componentGeneratorsElement.elementsByTagName(QStringLiteral("ipxact:componentGenerator"));
        for (int generatorIndex = 0; generatorIndex < generatorNodeList.count(); ++generatorIndex)
        {
            QDomNode generatorNode = generatorNodeList.at(generatorIndex);
            QSharedPointer<ComponentGenerator> newComponentGenerator =
                generatorReader.createComponentGeneratorFrom(generatorNode);

            newComponent->getComponentGenerators()->append(newComponentGenerator);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseChoices()
//-----------------------------------------------------------------------------
void ComponentReader::parseChoices(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement choicesElement = componentNode.firstChildElement(QStringLiteral("ipxact:choices"));

    if (!choicesElement.isNull())
    {
        ChoiceReader choiceReader;

        QDomNodeList choiceNodeList = choicesElement.elementsByTagName(QStringLiteral("ipxact:choice"));
        for (int choiceIndex = 0; choiceIndex < choiceNodeList.count(); ++choiceIndex)
        {
            QDomNode choiceNode = choiceNodeList.at(choiceIndex);
            QSharedPointer<Choice> newChoice = choiceReader.createChoiceFrom(choiceNode);

            newComponent->getChoices()->append(newChoice);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseFileSets()
//-----------------------------------------------------------------------------
void ComponentReader::parseFileSets(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement fileSetsElement = componentNode.firstChildElement(QStringLiteral("ipxact:fileSets"));

    if (!fileSetsElement.isNull())
    {
        FileSetReader setReader;

        QDomNodeList fileSetNodeList = fileSetsElement.elementsByTagName(QStringLiteral("ipxact:fileSet"));
        for (int fileSetIndex = 0; fileSetIndex < fileSetNodeList.count(); ++fileSetIndex)
        {
            QDomNode fileSetNode = fileSetNodeList.at(fileSetIndex);
            QSharedPointer<FileSet> newFileSet = setReader.createFileSetFrom(fileSetNode);

            newComponent->getFileSets()->append(newFileSet);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseCPUs()
//-----------------------------------------------------------------------------
void ComponentReader::parseCPUs(QDomNode const& componentNode, QSharedPointer<Component> newComponent) const
{
    QDomElement cpusElement = componentNode.firstChildElement(QStringLiteral("ipxact:cpus"));

    if (!cpusElement.isNull())
    {
        CPUReader cpuReader;

        QDomNodeList cpuNodeList = cpusElement.elementsByTagName(QStringLiteral("ipxact:cpu"));
        for (int cpuIndex = 0; cpuIndex < cpuNodeList.count(); ++cpuIndex)
        {
            QDomNode cpuNode = cpuNodeList.at(cpuIndex);
            QSharedPointer<Cpu> newCPU = cpuReader.createCPUFrom(cpuNode);

            newComponent->getCpus()->append(newCPU);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseOtherClockDrivers()
//-----------------------------------------------------------------------------
void ComponentReader::parseOtherClockDrivers(QDomNode const& componentNode,
    QSharedPointer<Component> newComponent) const
{
    QDomElement otherClocksElement = componentNode.firstChildElement(QStringLiteral("ipxact:otherClockDrivers"));

    if (!otherClocksElement.isNull())
    {
        OtherClockDriverReader otherClockReader;

        QDomNodeList clockNodeList = otherClocksElement.elementsByTagName(QStringLiteral("ipxact:otherClockDriver"));
        for (int clockIndex = 0; clockIndex < clockNodeList.count(); ++clockIndex)
        {
            QDomNode clockNode = clockNodeList.at(clockIndex);
            QSharedPointer<OtherClockDriver> newClockDriver =
                otherClockReader.createOtherClockDriverFrom(clockNode);

            newComponent->getOtherClockDrivers()->append(newClockDriver);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseComponentExtensions()
//-----------------------------------------------------------------------------
void ComponentReader::parseComponentExtensions(QDomNode const& componentNode,
    QSharedPointer<Component> newComponent) const
{
    QDomNodeList extensionNodeList = componentNode.firstChildElement(QStringLiteral("ipxact:vendorExtensions")).childNodes();

    for (int extensionIndex = 0; extensionIndex < extensionNodeList.count(); ++extensionIndex)
    {
        QDomNode singleExtensionNode = extensionNodeList.at(extensionIndex);
        if (singleExtensionNode.nodeName() == QLatin1String("kactus2:properties"))
        {
            parseSwProperties(singleExtensionNode, newComponent);
        }
        else if (singleExtensionNode.nodeName() == QLatin1String("kactus2:systemViews"))
        {
            parseSystemViews(singleExtensionNode, newComponent);
        }
        else if (singleExtensionNode.nodeName() == QLatin1String("kactus2:comInterfaces"))
        {
            parseComInterfaces(singleExtensionNode, newComponent);
        }
        else if (singleExtensionNode.nodeName() == QLatin1String("kactus2:apiInterfaces"))
        {
            parseApiInterfaces(singleExtensionNode, newComponent);
        }
        else if (singleExtensionNode.nodeName() == QLatin1String("kactus2:sourceDirectories"))
        {
            parseFilesetSourceDirectories(singleExtensionNode, newComponent);
        }
        else if (singleExtensionNode.nodeName() == QLatin1String("kactus2:fileDependencies"))
        {
            parseFileDependencies(singleExtensionNode, newComponent);
        }
        else if (singleExtensionNode.nodeName() == QLatin1String("kactus2:author"))
        {
            parseAuthor(singleExtensionNode, newComponent);
        }
    }

    parseKactusAndVendorExtensions(componentNode, newComponent);
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseSwProperties()
//-----------------------------------------------------------------------------
void ComponentReader::parseSwProperties(QDomNode const& propertiesNode, QSharedPointer<Component> newComponent)
    const
{
    QList<QSharedPointer<ComProperty> > swProperties;

    QDomNodeList propertyNodeList = propertiesNode.childNodes();
    for (int propertyIndex = 0; propertyIndex < propertyNodeList.count(); ++propertyIndex)
    {
        QDomElement propertyElement = propertyNodeList.at(propertyIndex).toElement();

        QString name = propertyElement.attribute(QStringLiteral("name"));
        QString type = propertyElement.attribute(QStringLiteral("propertyType"));
        QString defaultValue = propertyElement.attribute(QStringLiteral("defaultValue"));
        QString description =propertyElement.attribute(QStringLiteral("description"));
        QString requiredString = propertyElement.attribute(QStringLiteral("required"));
        bool required = false;
        if (requiredString == QLatin1String("true"))
        {
            required = true;
        }

        QSharedPointer<ComProperty> newSWProperty (new ComProperty());
        newSWProperty->setName(name);
        newSWProperty->setType(type);
        newSWProperty->setDefaultValue(defaultValue);
        newSWProperty->setDescription(description);
        newSWProperty->setRequired(required);

        swProperties.append(newSWProperty);
    }

    newComponent->setSWProperties(swProperties);
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseSystemViews()
//-----------------------------------------------------------------------------
void ComponentReader::parseSystemViews(QDomNode const& viewsNode, QSharedPointer<Component> newComponent) const
{
    QList<QSharedPointer<SystemView> > viewList;
    NameGroupReader nameReader;

    QDomNodeList viewNodeList = viewsNode.childNodes();
    for (int viewIndex = 0; viewIndex < viewNodeList.count(); ++viewIndex)
    {
        QDomElement singleSystemElement = viewNodeList.at(viewIndex).toElement();

        QSharedPointer<SystemView> newSystemView (new SystemView());

        nameReader.parseNameGroup(singleSystemElement, newSystemView);

        QDomElement hierarchyElement = singleSystemElement.firstChildElement(QStringLiteral("kactus2:hierarchyRef"));
        if (!hierarchyElement.isNull())
        {
            VLNV hierarhcyReference = parseVLNVAttributes(hierarchyElement, VLNV::DESIGN);
            newSystemView->setHierarchyRef(hierarhcyReference);
        }

        QDomElement hwViewReferenceElement = singleSystemElement.firstChildElement(QStringLiteral("kactus2:hwViewRef"));
        if (!hwViewReferenceElement.isNull())
        {
            QString viewRefernce = hwViewReferenceElement.firstChild().nodeValue();
            newSystemView->setHWViewRef(viewRefernce);
        }

        QDomNodeList fileSetRefNodeList = singleSystemElement.elementsByTagName(QStringLiteral("kactus2:fileSetRef"));
        QStringList fileSetReferences;
        for (int setIndex = 0; setIndex < fileSetRefNodeList.count(); ++setIndex)
        {
            QString setReference = fileSetRefNodeList.at(setIndex).firstChild().nodeValue();
            fileSetReferences.append(setReference);
        }
        newSystemView->setFileSetRefs(fileSetReferences);

        viewList.append(newSystemView);
    }

    newComponent->setSystemViews(viewList);
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseComInterfaces()
//-----------------------------------------------------------------------------
void ComponentReader::parseComInterfaces(QDomNode const& interfaceNode, QSharedPointer<Component> newComponent)
    const
{
    QList<QSharedPointer<ComInterface> > interfaceList;
    NameGroupReader nameReader;

    QDomNodeList interfaceNodeList = interfaceNode.childNodes();
    for (int interfaceIndex = 0; interfaceIndex < interfaceNodeList.count(); ++interfaceIndex)
    {
        QDomElement interfaceElement = interfaceNodeList.at(interfaceIndex).toElement();

        QSharedPointer<ComInterface> newComInterface (new ComInterface());

        nameReader.parseNameGroup(interfaceElement, newComInterface);

        QDomElement comTypeElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:comType"));
        if (!comTypeElement.isNull())
        {
            VLNV comType = parseVLNVAttributes(comTypeElement, VLNV::DESIGN);
            newComInterface->setComType(comType);
        }

        QDomElement transferElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:transferType"));
        if (!transferElement.isNull())
        {
            QString transfer = transferElement.firstChild().nodeValue();
            newComInterface->setTransferType(transfer);
        }

        QDomElement directionElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:comDirection"));
        if (!directionElement.isNull())
        {
            QString directionString = directionElement.firstChild().nodeValue();
            DirectionTypes::Direction direction =
                DirectionTypes::str2Direction(directionString, DirectionTypes::DIRECTION_INVALID);
            newComInterface->setDirection(direction);
        }

        QDomElement propertiesElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:propertyValues"));
        if (!propertiesElement.isNull())
        {
            QMap<QString, QString> newProperties;

            QDomNodeList propertiesNodeList = propertiesElement.childNodes();
            for (int i = 0; i < propertiesNodeList.count(); ++i)
            {
                QString name = propertiesNodeList.at(i).toElement().attribute(QStringLiteral("name"));
                QString value = propertiesNodeList.at(i).toElement().attribute(QStringLiteral("value"));
                newProperties.insert(name, value);
            }

            newComInterface->setPropertyValues(newProperties);
        }

        QDomElement comImplementationElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:comImplementationRef"));
        if (!comImplementationElement.isNull())
        {
            VLNV comImplementation = parseVLNVAttributes(comImplementationElement, VLNV::DESIGN);
            newComInterface->setComImplementation(comImplementation);
        }

        QDomElement positionElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:position"));
        if (!positionElement.isNull())
        {
            int positionX = positionElement.attribute(QStringLiteral("x")).toInt();
            int positionY = positionElement.attribute(QStringLiteral("y")).toInt();
            newComInterface->setDefaultPos(QPointF(positionX, positionY));
        }

        interfaceList.append(newComInterface);
    }

    newComponent->setComInterfaces(interfaceList);
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseApiInterfaces()
//-----------------------------------------------------------------------------
void ComponentReader::parseApiInterfaces(QDomNode const& interfaceNode, QSharedPointer<Component> newComponent)
    const
{
    QList<QSharedPointer<ApiInterface> > interfaceList;

    NameGroupReader nameReader;

    QDomNodeList interfaceNodeList = interfaceNode.childNodes();
    for (int interfaceIndex = 0; interfaceIndex < interfaceNodeList.count(); ++interfaceIndex)
    {
        QDomElement interfaceElement = interfaceNodeList.at(interfaceIndex).toElement();

        QSharedPointer<ApiInterface> newApiInterface (new ApiInterface());

        nameReader.parseNameGroup(interfaceElement, newApiInterface);

        QDomElement apiTypeElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:apiType"));
        if (!apiTypeElement.isNull())
        {
            VLNV apiType = parseVLNVAttributes(apiTypeElement, VLNV::DESIGN);
            newApiInterface->setApiType(apiType);
        }

        QDomElement directionElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:dependencyDirection"));
        if (!directionElement.isNull())
        {
            QString directionString = directionElement.firstChild().nodeValue();
            newApiInterface->setDependencyDirection
                (str2DependencyDirection(directionString, DEPENDENCY_PROVIDER));
        }

        QDomElement positionElement = interfaceElement.firstChildElement(QStringLiteral("kactus2:position"));
        if (!positionElement.isNull())
        {
            int positionX = positionElement.attribute(QStringLiteral("x")).toInt();
            int positionY = positionElement.attribute(QStringLiteral("y")).toInt();
            newApiInterface->setDefaultPos(QPointF(positionX, positionY));
        }

        interfaceList.append(newApiInterface);
    }

    newComponent->setApiInterfaces(interfaceList);
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseFilesetSourceDirectories()
//-----------------------------------------------------------------------------
void ComponentReader::parseFilesetSourceDirectories(QDomNode const& singleExtensionNode, 
    QSharedPointer<Component> newComponent) const
{
    QStringList sourceDirectories;

    QDomNodeList dirctoryNodeList = singleExtensionNode.childNodes();
    for (int i = 0; i < dirctoryNodeList.count(); i++)
    {
        sourceDirectories.append(dirctoryNodeList.at(i).firstChild().nodeValue());
    }

    newComponent->setSourceDirectories(sourceDirectories);
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseFileDependencies()
//-----------------------------------------------------------------------------
void ComponentReader::parseFileDependencies(QDomNode const& fileNode, QSharedPointer<Component> newComponent) const
{
    QList<QSharedPointer<FileDependency> > fileDependencies;

    QDomNodeList dependencyNodeList = fileNode.childNodes();
    for (int dependencyIndex = 0; dependencyIndex < dependencyNodeList.count(); ++dependencyIndex)
    {
        QDomElement dependencyElement = dependencyNodeList.at(dependencyIndex).toElement();

        QString file1 = dependencyElement.firstChildElement(QStringLiteral("kactus2:fileRef1")).firstChild().nodeValue();
        QString file2 = dependencyElement.firstChildElement(QStringLiteral("kactus2:fileRef2")).firstChild().nodeValue();
        QString description = dependencyElement.firstChildElement(QStringLiteral("ipxact:description")).firstChild().nodeValue();

        bool locked = false;
        if (dependencyElement.attribute(QStringLiteral("locked")) == QLatin1String("true"))
        {
            locked = true;
        }

        bool biDirectional = false;
        if (dependencyElement.attribute(QStringLiteral("bidirectional")) == QLatin1String("true"))
        {
            biDirectional = true;
        }

        bool manual = false;
        if (dependencyElement.attribute(QStringLiteral("manual")) == QLatin1String("true"))
        {
            manual = true;
        }
        
        QSharedPointer<FileDependency> newDependency (new FileDependency());
        newDependency->setFile1(file1);
        newDependency->setFile2(file2);
        newDependency->setDescription(description);
        newDependency->setLocked(locked);
        newDependency->setBidirectional(biDirectional);
        newDependency->setManual(manual);

        fileDependencies.append(newDependency);
    }

    newComponent->setFileDependendencies(fileDependencies);
}

//-----------------------------------------------------------------------------
// Function: ComponentReader::parseAuthor()
//-----------------------------------------------------------------------------
void ComponentReader::parseAuthor(QDomNode const& authorNode, QSharedPointer<Component> newComponent) const
{
    newComponent->setAuthor(authorNode.firstChild().nodeValue());
}
