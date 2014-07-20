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
 *	\file TestOgr.cpp
 *	\brief Qt Test Ogr
 *	\author David Tran [ HSR ]
 *	\version 0.1
 *	\date 13/06/14
 */

#include "TestOgr.h"

TestOgr::TestOgr() {
    ogr = new Ogr();
    path = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QDir::separator() + "test" + QDir::separator()).toStdString();
    filename = "poly.shp";
    sqlitedb = "poly.sqlite";
}

void TestOgr::testOpenWFS() {
    QString uri = "";
    QStringList fileList;
    bool resVal = ogr->OpenWFS(uri, fileList);
    QCOMPARE(resVal, false);
}

void TestOgr::testOpenSourceFalseInput() {
    string epsg;
    string query;
    string error;
    bool resVal = ogr->OpenSource(filename, epsg, query, error);
    QCOMPARE(resVal, false);
}

void TestOgr::testCloseSourceFalseInput() {
    bool resVal = ogr->CloseSource();
    QCOMPARE(resVal, false);
}

void TestOgr::testOpenSourceFile() {
    string sourcename = path + filename;
    string epsg;
    string query;
    string error;
    bool resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
}

void TestOgr::testOpenSourceSQLite() {
    string sourcename = path + sqlitedb;
    string epsg;
    string query;
    string error;
    bool resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
}

void TestOgr::testCloseSource() {
    bool resVal = ogr->CloseSource();
    QCOMPARE(resVal, true);
}

void TestOgr::testOpenDriverFalseInput() {
    bool resVal = ogr->OpenDriver("");
    QCOMPARE(resVal, false);
}

void TestOgr::testOpenDriverESRIShapefile() {
    string drivername = "ESRI Shapefile";
    bool resVal = ogr->OpenDriver(drivername);
    QCOMPARE(resVal, true);
}

void TestOgr::setSource(string sourcename) {
    sourceData = OGROpen(sourcename.c_str(), 0, NULL);
    if(sourceData != NULL) {
        sourceLayer = OGR_DS_GetLayer(sourceData, 0);
        OGR_L_ResetReading(sourceLayer);
        if(sourceLayer != NULL) {
            sourceLayerDefn = OGR_L_GetLayerDefn(sourceLayer);
            sourceLayerName = OGR_FD_GetName(sourceLayerDefn);
        }
    }
}

void TestOgr::testLayerNames() {
    string epsg;
    string query;
    string error;

    string sourcename = path + filename;
    bool resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->OpenDriver("ESRI Shapefile");
    QCOMPARE(resVal, true);
    setSource(sourcename);
    string layerNameShapefile = sourceLayerName;
    resVal = ogr->CloseSource();
    QCOMPARE(resVal, true);

    sourcename = path + sqlitedb;
    resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->OpenDriver("SQLite");
    QCOMPARE(resVal, true);
    setSource(sourcename);
    string layerNameSQLite = sourceLayerName;
    resVal = ogr->CloseSource();
    QCOMPARE(resVal, true);

    QCOMPARE(layerNameShapefile, layerNameSQLite);
}

void TestOgr::testLayerCount() {
    string epsg;
    string query;
    string error;

    string sourcename = path + filename;
    bool resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->OpenDriver("ESRI Shapefile");
    QCOMPARE(resVal, true);
    setSource(sourcename);
    int layerCountShapefile = OGR_DS_GetLayerCount(sourceData);
    resVal = ogr->CloseSource();
    QCOMPARE(resVal, true);

    sourcename = path + sqlitedb;
    resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->OpenDriver("SQLite");
    QCOMPARE(resVal, true);
    setSource(sourcename);
    int layerCountSQLite = OGR_DS_GetLayerCount(sourceData);
    resVal = ogr->CloseSource();
    QCOMPARE(resVal, true);

    QCOMPARE(layerCountShapefile, layerCountSQLite);
}

void TestOgr::testFeatureCount() {
    string epsg;
    string query;
    string error;

    string sourcename = path + filename;
    bool resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->OpenDriver("ESRI Shapefile");
    QCOMPARE(resVal, true);
    setSource(sourcename);
    int featureCountShapefile = OGR_L_GetFeatureCount(sourceLayer, true);
    resVal = ogr->CloseSource();
    QCOMPARE(resVal, true);

    sourcename = path + sqlitedb;
    resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->OpenDriver("SQLite");
    QCOMPARE(resVal, true);
    setSource(sourcename);
    int featureCountSQLite = OGR_L_GetFeatureCount(sourceLayer, true);
    resVal = ogr->CloseSource();
    QCOMPARE(resVal, true);

    QCOMPARE(featureCountShapefile, featureCountSQLite);
}

void TestOgr::testSQLQueryFalseQuery() {
    string epsg;
    string query;
    string error;
    string sourcename = path + filename;
    bool resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->TestExecuteSQL("SELECT none FROM " + sourceLayerName);
    QCOMPARE(resVal, false);
    resVal = ogr->TestExecuteSQL("SELECT * FROM polyy");
    QCOMPARE(resVal, false);
}

void TestOgr::testSQLQuery() {
    string epsg;
    string query;
    string error;
    string sourcename = path + filename;
    bool resVal = ogr->OpenSource(sourcename, epsg, query, error);
    QCOMPARE(resVal, true);
    resVal = ogr->TestExecuteSQL("SELECT * FROM " + sourceLayerName);
    QCOMPARE(resVal, true);
    resVal = ogr->TestExecuteSQL("SELECT area FROM " + sourceLayerName);
    QCOMPARE(resVal, true);
    resVal = ogr->TestExecuteSQL("SELECT eas_id FROM " + sourceLayerName);
    QCOMPARE(resVal, true);
    resVal = ogr->TestExecuteSQL("SELECT prfedea FROM " + sourceLayerName);
    QCOMPARE(resVal, true);
}
