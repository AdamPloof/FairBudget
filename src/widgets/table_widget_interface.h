#ifndef TABLE_WIDGET_INTERFACE_H
#define TABLE_WIDGET_INTERFACE_H

#include <memory>
#include "../models/model_interface.h"

class TableWidgetInterface {
public:
    virtual void addRow(std::shared_ptr<ModelInterface> row) = 0;
};

#endif // TABLE_WIDGET_INTERFACE_H
