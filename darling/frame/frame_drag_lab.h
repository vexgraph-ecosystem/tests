#ifndef DARLING_TEST_FRAME_DRAG_LAB_H
#define DARLING_TEST_FRAME_DRAG_LAB_H

#include "frame/frame.h"
#include "c23/event_invoke.h"

// Demo-local drag controller. The root receives bubbled events so moving off
// the selected panel (but still inside the frame) does not lose the drag.
// This is not toolkit pointer capture outside the native window.
typedef struct FrameDragLab {
    Frame *frame;
    Panel **panels;
    int count, selected;
    float x[96], y[96], startX, startY, baseX, baseY;
} FrameDragLab;

static void frameDragDown(Element *element, const Mouse *mouse, void *userdata) {
    FrameDragLab *drag = userdata;
    (*drag).selected = -1;
    Element *hit = Element_hit(element, (*mouse).x, (*mouse).y);
    for (int i = 0; i < (*drag).count; ++i) {
        if (Panel_graphics((*drag).panels[i]) != hit) continue;
        (*drag).selected = i;
        (*drag).startX = (*mouse).x;
        (*drag).startY = (*mouse).y;
        (*drag).baseX = (*drag).x[i];
        (*drag).baseY = (*drag).y[i];
        break;
    }
}

static void frameDragMove(Element *element, const Mouse *mouse, void *userdata) {
    (void)element;
    FrameDragLab *drag = userdata;
    int i = (*drag).selected;
    if (i < 0) return;
    (*drag).x[i] = (*drag).baseX + (*mouse).x - (*drag).startX;
    (*drag).y[i] = (*drag).baseY + (*mouse).y - (*drag).startY;
    Panel_setOffset((*drag).panels[i], (*drag).x[i], (*drag).y[i]);
    Frame_invalidate((*drag).frame);
}

static void frameDragUp(Element *element, const Mouse *mouse, void *userdata) {
    (void)element; (void)mouse;
    FrameDragLab *drag = userdata;
    (*drag).selected = -1;
}

static void frameDragAttach(FrameDragLab *drag) {
    (*drag).selected = -1;
    Element_addMouseEvent(Frame_element((*drag).frame), (MouseEvent){
        .onDown = frameDragDown, .onDrag = frameDragMove, .onUp = frameDragUp,
        .userdata = drag,
    });
}

#endif
