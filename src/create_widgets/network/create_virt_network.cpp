#include "create_virt_network.h"

/*
 * INTERFACE TYPES:
 * network
 * bridge
 * direct
 * user             ( for usermod VMs only [KVM-?] )
 * - ethernet       ( requires special knowledge the user )
 * hostdev
 * mcast
 * server/client
 */

CreateVirtNetwork::CreateVirtNetwork(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Network Settings");
    restoreGeometry(settings.value("NetCreateGeometry").toByteArray());
    bridgeLayout = new QGridLayout();
    bridgeName = new QLineEdit(this);
    bridgeName->setPlaceholderText("Enter bridge name");
    stp = new QCheckBox("Spanning Tree Protocol", this);
    stp->setChecked(true); // default state
    delayLabel = new QLabel("Delay", this);
    delay = new QSpinBox(this);
    delay->setRange(0, 360);
    bridgeLayout->addWidget(bridgeName, 0, 0, 1, 2);
    bridgeLayout->addWidget(stp, 2, 0, 3, 2);
    bridgeLayout->addWidget(delayLabel, 4, 0, 5, 1);
    bridgeLayout->addWidget(delay, 4, 1, 5, 2);
    bridge = new QWidget(this);
    bridge->setLayout(bridgeLayout);

    netDescLayout = new QVBoxLayout();
    networkName = new QLineEdit(this);
    networkName->setPlaceholderText("Enter Network Name");
    uuid = new QLabel("UUID:");


    ok = new QPushButton("Ok", this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    cancel = new QPushButton("Cancel", this);
    cancel->setAutoDefault(true);
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    netDescLayout->addWidget(networkName);
    netDescLayout->addWidget(uuid);
    netDescLayout->addWidget(bridge);
    netDescLayout->insertStretch(-1);
    netDescLayout->addWidget(buttons);
    setLayout(netDescLayout);

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml").arg(QDir::tempPath()).arg(QDir::separator()));
}
CreateVirtNetwork::~CreateVirtNetwork()
{
    settings.setValue("NetCreateGeometry", saveGeometry());
    disconnect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    disconnect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    delete networkName;
    networkName = 0;
    delete uuid;
    uuid = 0;
    delete bridgeName;
    bridgeName = 0;
    delete stp;
    stp = 0;
    delete delayLabel;
    delayLabel = 0;
    delete delay;
    delay = 0;
    delete bridgeLayout;
    bridgeLayout = 0;
    delete bridge;
    bridge = 0;

    delete ok;
    ok = 0;
    delete cancel;
    cancel = 0;
    delete buttonLayout;
    buttonLayout = 0;
    delete buttons;
    buttons = 0;

    delete netDescLayout;
    netDescLayout = 0;

    delete xml;
    xml = 0;
}

/* public slots */
QString CreateVirtNetwork::getXMLDescFileName() const
{
    return xml->fileName();
}

/* private slots */
void CreateVirtNetwork::buildXMLDescription()
{

}
void CreateVirtNetwork::set_Result()
{
    if ( sender()==ok ) {
        setResult(QDialog::Accepted);
        buildXMLDescription();
    } else {
        setResult(QDialog::Rejected);
    };
    done(result());
}
