#ifndef FLOWGRAPHCONSTANTS_H
#define FLOWGRAPHCONSTANTS_H

#include <QColor>

// NODE
const int NODE_WIDTH = 160;
const int NODE_HEADER_HEIGHT = 40;
const int NODE_HEIGHT_PER_INPUT = 30;
const int NODE_SELECTION_WIDTH = 5;
const QColor NODE_EDGE_COLOR = QColor(156, 155, 154);
const QColor NODE_SELECTED_EDGE_COLOR = QColor(250, 100, 100);
const QColor NODE_BACKGROUND_COLOR = QColor(186, 184, 175);
const QColor NODE_HEADER_BACKGROUND_COLOR = QColor(186, 184, 175);
const QColor NODE_PINS_BACKGROUND_COLOR = QColor(205, 205, 205);
const QColor NODE_TEXT_COLOR = QColor(0,0,0);
const QColor CLEAR_COLOR = QColor(255, 0, 0);

// PIN
const int NODE_PIN_MARGIN = 5;
const int NODE_PIN_TOP_MARGIN = 5;
static const int PIN_RADIUS = 15;
const int DATA_PIN_TYPE_TEXT_WIDTH = 5;
const int NODE_PIN_TEXT_WIDTH = NODE_WIDTH/2 - NODE_PIN_MARGIN - PIN_RADIUS;
static const QColor PIN_INT_COLOR = QColor(255,0,0);
static const QColor PIN_FLOAT_COLOR = QColor(0,255,0);
static const QColor PIN_STRING_COLOR = QColor(0,0,255);
static const QColor PIN_IMAGE_COLOR = QColor(0,255,255);

#endif // FLOWGRAPHCONSTANTS_H
