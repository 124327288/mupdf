#include "fitz.h"
#include "mupdf-internal.h"

typedef struct
{
	fz_doc_event base;
	fz_alert_event alert;
} fz_alert_event_internal;

fz_alert_event *fz_access_alert_event(fz_doc_event *event)
{
	fz_alert_event *alert = NULL;

	if (event->type == FZ_DOCUMENT_EVENT_ALERT)
		alert = &((fz_alert_event_internal *)event)->alert;

	return alert;
}

void pdf_event_issue_alert(pdf_document *doc, fz_alert_event *alert)
{
	if (doc->event_cb)
	{
		fz_alert_event_internal ievent;
		ievent.base.type = FZ_DOCUMENT_EVENT_ALERT;
		ievent.alert = *alert;

		doc->event_cb((fz_doc_event *)&ievent, doc->event_cb_data);

		*alert = ievent.alert;
	}
}

void pdf_event_issue_print(pdf_document *doc)
{
	fz_doc_event e;

	e.type = FZ_DOCUMENT_EVENT_PRINT;

	if (doc->event_cb)
		doc->event_cb(&e, doc->event_cb_data);
}

typedef struct
{
	fz_doc_event base;
	char *item;
} fz_exec_menu_item_event_internal;

char *fz_access_exec_menu_item_event(fz_doc_event *event)
{
	char *item = NULL;

	if (event->type == FZ_DOCUMENT_EVENT_EXEC_MENU_ITEM)
		item = ((fz_exec_menu_item_event_internal *)event)->item;

	return item;
}

void pdf_event_issue_exec_menu_item(pdf_document *doc, char *item)
{
	if (doc->event_cb)
	{
		fz_exec_menu_item_event_internal ievent;
		ievent.base.type = FZ_DOCUMENT_EVENT_EXEC_MENU_ITEM;
		ievent.item = item;

		doc->event_cb((fz_doc_event *)&ievent, doc->event_cb_data);
	}
}

void pdf_set_doc_event_callback(pdf_document *doc, fz_doc_event_cb *fn, void *data)
{
	doc->event_cb = fn;
	doc->event_cb_data = data;
}
