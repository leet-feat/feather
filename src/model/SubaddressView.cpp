// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020-2021, The Monero Project.

#include "SubaddressView.h"

#include "model/ModelUtils.h"

SubaddressView::SubaddressView(QWidget *parent) : QTreeView(parent) {

}

void SubaddressView::keyPressEvent(QKeyEvent *event){
    QModelIndexList selectedRows = selectionModel()->selectedRows();

    if(!selectedIndexes().isEmpty()){
        if(event->matches(QKeySequence::Copy)){
            QModelIndex index = this->currentIndex();
            ModelUtils::copyColumn(&index, index.column());
        }
        else
            QTreeView::keyPressEvent(event);
    }
}
