// SPDX-License-Identifier: GPL-2.0
// Displays the dive profile. Used by the interactive profile widget
// and the printing/exporting code.
#ifndef PROFILESCENE_H
#define PROFILESCENE_H

#include "core/color.h"
#include "core/display.h"

#include <QGraphicsScene>
#include <QPainter>
#include <memory>

class DivePlannerPointsModel;
class DivePlotDataModel;

class AbstractProfilePolygonItem;
class DiveCalculatedCeiling;
class DiveCalculatedTissue;
class DiveCartesianAxis;
class DiveEventItem;
class DiveGasPressureItem;
class DiveHeartrateItem;
class DiveMeanDepthItem;
class DivePercentageItem;
class DivePixmaps;
class DiveProfileItem;
class DiveReportedCeiling;
class DiveTemperatureItem;
class DiveTextItem;
class PartialPressureGasItem;
class TankItem;

class ProfileScene : public QGraphicsScene {
public:
	ProfileScene(double dpr, bool printMode, bool isGrayscale);
	~ProfileScene();

	void resize(QSizeF size);
	void clear();
	bool isPointOutOfBoundaries(const QPointF &point) const;

	// If a plannerModel is passed, the deco-information is taken from there.
	void plotDive(const struct dive *d, int dc, DivePlannerPointsModel *plannerModel = nullptr, bool inPlanner = false,
		      bool instant = false, bool calcMax = true, double zoom = 1.0, double zoomedPosition = 0.0);

	void draw(QPainter *painter, const QRect &pos,
		  const struct dive *d, int dc,
		  DivePlannerPointsModel *plannerModel = nullptr, bool inPlanner = false);

	const struct dive *d;
	int dc;
private:
	template<typename T, class... Args> T *createItem(const DiveCartesianAxis &vAxis, int vColumn, int z, Args&&... args);
	PartialPressureGasItem *createPPGas(int column, color_index_t color, color_index_t colorAlert,
					    const double *thresholdSettingsMin, const double *thresholdSettingsMax);
	void updateVisibility(bool diveHasHeartBeat); // Update visibility of non-interactive chart features according to preferences
	void updateAxes(bool diveHasHeartBeat); // Update axes according to preferences

	friend class ProfileWidget2; // For now, give the ProfileWidget full access to the objects on the scene
	double dpr; // Device Pixel Ratio. A DPR of one corresponds to a "standard" PC screen.
	bool printMode;
	bool isGrayscale;
	int maxtime;
	int maxdepth;

	DivePlotDataModel *dataModel;
	struct plot_info plotInfo;
	DiveCartesianAxis *profileYAxis;
	DiveCartesianAxis *gasYAxis;
	DiveCartesianAxis *temperatureAxis;
	DiveCartesianAxis *timeAxis;
	DiveCartesianAxis *cylinderPressureAxis;
	DiveCartesianAxis *heartBeatAxis;
	DiveCartesianAxis *percentageAxis;
	std::vector<AbstractProfilePolygonItem *> profileItems;
	DiveProfileItem *diveProfileItem;
	DiveTemperatureItem *temperatureItem;
	DiveMeanDepthItem *meanDepthItem;
	DiveGasPressureItem *gasPressureItem;
	QList<DiveEventItem *> eventItems;
	DiveTextItem *diveComputerText;
	DiveReportedCeiling *reportedCeiling;
	PartialPressureGasItem *pn2GasItem;
	PartialPressureGasItem *pheGasItem;
	PartialPressureGasItem *po2GasItem;
	PartialPressureGasItem *o2SetpointGasItem;
	PartialPressureGasItem *ccrsensor1GasItem;
	PartialPressureGasItem *ccrsensor2GasItem;
	PartialPressureGasItem *ccrsensor3GasItem;
	PartialPressureGasItem *ocpo2GasItem;
	DiveCalculatedCeiling *diveCeiling;
	DiveTextItem *decoModelParameters;
	QList<DiveCalculatedTissue *> allTissues;
	DiveHeartrateItem *heartBeatItem;
	DivePercentageItem *percentageItem;
	TankItem *tankItem;
	std::shared_ptr<const DivePixmaps> pixmaps;
};

#endif