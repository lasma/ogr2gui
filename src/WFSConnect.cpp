/*****************************************************************************
 * ogr2gui is an application used to convert and manipulate geospatial
 * data. It is based on the "OGR Simple Feature Library" from the
 * "Geospatial Data Abstraction Library" <http://gdal.org>.
 *
 * Copyright (c) 2014 Faculty of Computer Science,
 * University of Applied Sciences Rapperswil (HSR),
 * 8640 Rapperswil, Switzerland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

/*!
 *	\file wfsConnect.cpp
 *	\brief WFS Connection
 *	\author David Tran [ HSR ]
 *	\version 0.7
 *	\date 13/06/14
 */

#include "wfsConnect.h"

WFSConnect::WFSConnect( QWidget *parent ) : QDialog( parent )
{
    InitInterface();
    InitSlots();
    TranslateInterface();

    this->setWindowModality( Qt::ApplicationModal );
    this->setMinimumWidth( 380 );
}

WFSConnect::~WFSConnect( void )
{

}

void WFSConnect::InitInterface( void )
{
    theLayout = new QVBoxLayout( this );
    {
        lytInfo = new QGridLayout();
        {
            lblHost = new QLabel();
            lblHost->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
            lblHost->setMinimumSize( 70, 20 );
            lblHost->setMaximumSize( 70, 20 );

            lytHost = new QHBoxLayout();
            {
                txtHost = new QLineEdit();
                lytHost->addWidget( txtHost );
            }

            lytInfo->addWidget( lblHost, 0, 0 );
            lytInfo->addLayout( lytHost, 0, 1 );

            btnConnect = new QPushButton();

            lytInfo->addWidget( btnConnect, 4, 1 );

            lytTables = new QVBoxLayout();
            {
                lblTables = new QLabel();
                lblTables->setAlignment( Qt::AlignRight );
                lblTables->setMinimumSize( 70, 20 );
                lblTables->setMaximumSize( 70, 20 );

                radAllTables = new QRadioButton();
                radAllTables->setLayoutDirection( Qt::RightToLeft );

                radNonTables = new QRadioButton();
                radNonTables->setLayoutDirection( Qt::RightToLeft );
                radNonTables->setChecked( true );

                lytTables->addWidget( lblTables );
                lytTables->addWidget( radAllTables );
                lytTables->addWidget( radNonTables );
            }

            lstTables = new QListWidget();

            lytInfo->addLayout( lytTables, 6, 0 );
            lytInfo->addWidget( lstTables, 6, 1 );
        }

        theLayout->addLayout( lytInfo );

        lytDialog = new QHBoxLayout();
        {
            btnAccept = new QPushButton();
            btnAccept->setEnabled( false );

            btnCancel = new QPushButton();

            lytDialog->addWidget( btnAccept );
            lytDialog->addWidget( btnCancel );
        }

        theLayout->addLayout( lytDialog );
    }

    this->setLayout( theLayout );
}

void WFSConnect::InitSlots( void )
{
    QObject::connect( btnConnect, SIGNAL( clicked() ), this, SLOT( evtBtnConnect( void ) ) );
    QObject::connect( radAllTables, SIGNAL( clicked() ), this, SLOT( evtRadAllLayers( void ) ) );
    QObject::connect( radNonTables, SIGNAL( clicked() ), this, SLOT( evtRadNonLayers( void ) ) );
    QObject::connect( btnCancel, SIGNAL( clicked() ), this, SLOT( evtBtnCancel( void ) ) );
    QObject::connect( btnAccept, SIGNAL( clicked() ), this, SLOT( evtBtnAccept( void ) ) );
}

void WFSConnect::TranslateInterface( void )
{
    this->setWindowTitle( tr( "Web feature service" ) );

    lblHost->setText( tr( "URI" ) );

    btnConnect->setText( tr( "Connect" ) );

    lblTables->setText( tr( "Layers" ) );

    radAllTables->setText( tr( "all" ) );
    radNonTables->setText( tr( "none" ) );

    btnAccept->setText( tr( "Accept" ) );
    btnCancel->setText( tr( "Cancel" ) );
}

void WFSConnect::evtBtnConnect( void )
{
    lstTables->clear();
    QMessageBox msg;
    Ogr ogr;
    QStringList fileList;
    QString filename = connectionType + txtHost->text();
    if(ogr.OpenWFS(filename, fileList)) {
        QStringList::Iterator it = fileList.begin();
        while(it != fileList.end()) {
            QListWidgetItem *item = new QListWidgetItem(*it);
            item->setCheckState( Qt::Unchecked );
            lstTables->addItem( item );
            ++it;
        }
        btnAccept->setEnabled( true );
    } else {
        msg.setText( "* can't connect to web service !" );
        msg.exec();
    }
}

void WFSConnect::evtRadAllLayers( void )
{
    for( int i = 0; i < lstTables->count(); i ++ )
    {
        lstTables->item( i )->setCheckState( Qt::Checked );
    }
}

void WFSConnect::evtRadNonLayers( void )
{
    for( int i = 0; i < lstTables->count(); i ++ )
    {
        lstTables->item( i )->setCheckState( Qt::Unchecked );
    }
}

void WFSConnect::evtBtnAccept( void )
{
    connectionString = txtHost->text();
    selectedLayers.clear();
    selectedLayersList.clear();
    for(int i = 0; i < lstTables->count(); ++i) {
        if(lstTables->item(i)->checkState() == Qt::Checked) {
            selectedLayers += lstTables->item(i)->text() + tr(" ");
        }
    }
    selectedLayers = selectedLayers.simplified();
    selectedLayersList = selectedLayers.split(" ");

    lstTables->clear();
    btnAccept->setEnabled( false );
    radNonTables->setChecked( true );
    this->accept();
}

void WFSConnect::evtBtnCancel( void )
{
    lstTables->clear();
    btnAccept->setEnabled( false );
    radNonTables->setChecked( true );
    this->reject();
}

void WFSConnect::setConnectionType( QString type )
{
    connectionType = type;
}

QString WFSConnect::getConnectionString( void ) const
{
    return connectionString;
}

QString WFSConnect::getSelectedLayers( void ) const
{
    return selectedLayers;
}

QStringList WFSConnect::getSelectedLayersAsList( void ) const
{
    return selectedLayersList;
}
