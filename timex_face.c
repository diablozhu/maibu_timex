/*
 * =====================================================================================
 *
 *       Filename:  timex_face.c
 *         Author:  diablozhu , diablozhu@gmail.com
 *        Created:  2016/01/05
 *
 *    Description:
 *        timex ws4 style, edited with notepad++
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "maibu_sdk.h"
#include "maibu_res.h"

/* 时间 */
#define TIME_ORIGIN_X 7
#define TIME_ORIGIN_Y 41
#define TIME_HEIGHT   26
#define TIME_WIDTH   111

/*日期图层*/
#define DATE_ORIGIN_X   7
#define DATE_ORIGIN_Y   68
#define DATE_HEIGHT     17
#define DATE_WIDTH     111

/*蓝牙*/
#define BLE_ORIGIN_X     10
#define BLE_ORIGIN_Y     8
#define BLE_H       30
#define BLE_W       42

//海拔
#define HEIGHT_ORIGIN_X  59
#define HEIGHT_ORIGIN_Y  7
#define HEIGHT_H    12
#define HEIGHT_W    55

/*运动量*/
#define SPORT_ORIGIN_X  57
#define SPORT_ORIGIN_Y  89
#define SPORT_HEIGHT    16
#define SPORT_WIDTH    57

//楼层数
#define FLOOR_ORIGIN_X  7
#define FLOOR_ORIGIN_Y  89
#define FLOOR_H    16
#define FLOOR_W    47

//电量
#define BATTERY_ORIGIN_X  15
#define BATTERY_ORIGIN_Y  108
#define BATTERY_H    14
#define BATTERY_W    28


static int8_t g_app_window_id = -1;
static int8_t g_app_time_layer_id = -1;
static int8_t g_app_date_layer_id = -1;
static int8_t g_app_sport_layer_id = -1;
static int8_t g_app_floor_layer_id = -1;
static int8_t g_app_battery_layer_id = -1;
static int8_t g_app_ble_layer_id = -1;
static int8_t g_app_height_layer_id = -1;
static uint8_t app_timer_change_id = -1;

void display_bitmap(P_Window pwindow)
{
    if (pwindow == NULL)
        return;
	//创建底图

    GRect frame = {{ 0, 0}, {128, 128}}; 
    GBitmap bitmap;

    res_get_user_bitmap(RES_BITMAP_BACK, &bitmap);

    LayerBitmap layer_bitmap = {bitmap, frame, GAlignCenter};

    P_Layer layer = app_layer_create_bitmap(&layer_bitmap);
    if (layer != NULL) {
        app_window_add_layer(pwindow, layer);
    }
}

void get_time_layer_str(char *str)
{
    struct date_time t;
    app_service_get_datetime(&t);

    sprintf(str, "%d:%02d", t.hour, t.min);
}

void get_sport_layer_str(char *str1,char *str2)
{
	SportData data;
	if (0 == maibu_get_sport_data(&data, 0)) {
		sprintf(str1, "%d步", data.step);
		sprintf(str2, "%d层", data.activity);
	}
}

void get_height_layer_str(char* str)
{
	int8_t iok;
	float altitude,accuracy;
	iok = maibu_get_altitude (&altitude,&accuracy);
	if(iok==0){
		sprintf(str,"%gm",altitude);
	}
}

void get_date_layer_str(char *str)
{
	struct date_time w;
	app_service_get_datetime(&w);
	int num = w.wday;
	if (num == 0) {
		sprintf(str, "周日 %d-%02d", w.mon, w.mday);
	}
	if (num == 1) {
		sprintf(str, "周一 %d-%02d", w.mon, w.mday);
	}
	if (num == 2) {
		sprintf(str, "周二 %d-%02d", w.mon, w.mday);
	}
	if (num == 3) {
		sprintf(str, "周三 %d-%02d", w.mon, w.mday);
	}
	if (num == 4) {
		sprintf(str, "周四 %d-%02d", w.mon, w.mday);
	}
	if (num == 5) {
		sprintf(str, "周五 %d-%02d", w.mon, w.mday);
	}
	if (num == 6) {
		sprintf(str, "周六 %d-%02d", w.mon, w.mday);
	}
}

void get_battery_layer_str(char *str)
{
	int8_t percent;
	maibu_get_battery_percent(&percent);
	sprintf(str, "%d", percent);
}

void init_text_layer(P_Window pwindow)
{
	char str[30] = "";
	char str2[20] = "";
	//蓝牙图层
	GBitmap bitmap_ble_con;
	//创建蓝牙图层
	GRect frame_ble = { { BLE_ORIGIN_X,BLE_ORIGIN_Y },{ BLE_H,BLE_W } };
	res_get_user_bitmap(RES_BITMAP_BLE_CON, &bitmap_ble_con);
	LayerBitmap layer_bitmap_ble_con = { bitmap_ble_con,frame_ble,GAlignCenter };
	P_Layer layer_ble = app_layer_create_bitmap(&layer_bitmap_ble_con);
	if(layer_ble !=NULL)
		g_app_ble_layer_id = app_window_add_layer(pwindow, layer_ble);
	
	//高度图层
	GRect frame_height = {{HEIGHT_ORIGIN_X,HEIGHT_ORIGIN_Y},{HEIGHT_H,HEIGHT_W}};
	get_height_layer_str(str);
	LayerText lt_height = {str,frame_height,GAlignCenter,U_ASCII_ARIAL_12,0};
	P_Layer layer_ht = app_layer_create_text(&lt_height);
	if(layer_ht !=NULL){
		g_app_height_layer_id = app_window_add_layer(pwindow,layer_ht);
	}
	
	//时间图层
	GRect frame_time = { { TIME_ORIGIN_X, TIME_ORIGIN_Y },{ TIME_HEIGHT, TIME_WIDTH } };
    get_time_layer_str(str);
    LayerText lt_hm = {str, frame_time, GAlignCenter, U_ASCII_ARIALBD_24, 0};
    P_Layer layer_hm = app_layer_create_text(&lt_hm);
    if(layer_hm != NULL) {
        g_app_time_layer_id = app_window_add_layer(pwindow, layer_hm);
    }

	//日期图层
	GRect frame_date = { {DATE_ORIGIN_X,DATE_ORIGIN_Y},{DATE_HEIGHT,DATE_WIDTH} };
	get_date_layer_str(str);
	LayerText lt_date = { str, frame_date, GAlignCenter, U_ASCII_ARIAL_14, 0 };
	P_Layer layer_date = app_layer_create_text(&lt_date);
	if (layer_date != NULL) {
		g_app_date_layer_id = app_window_add_layer(pwindow, layer_date);
	}
	
	//楼层数
	GRect frame_floor = {{FLOOR_ORIGIN_X,FLOOR_ORIGIN_Y},{FLOOR_H,FLOOR_W}};

	//运动量和楼层
	GRect frame_sport = { { SPORT_ORIGIN_X,SPORT_ORIGIN_Y },{ SPORT_HEIGHT,SPORT_WIDTH } };
	get_sport_layer_str(str,str2);
	LayerText lt_sport = { str, frame_sport, GAlignCenter, U_ASCII_ARIAL_14, 0 };
	LayerText lt_floor = { str2, frame_floor, GAlignCenter, U_ASCII_ARIAL_14, 0 };
	P_Layer layer_sport = app_layer_create_text(&lt_sport);
	P_Layer layer_floor = app_layer_create_text(&lt_floor);
	if (layer_sport != NULL) {
		g_app_sport_layer_id = app_window_add_layer(pwindow, layer_sport);
	}
	if(layer_floor !=NULL){
		g_app_floor_layer_id = app_window_add_layer(pwindow,layer_floor);
	}

	//电池电量
	GRect frame_battery_text = { { BATTERY_ORIGIN_X, BATTERY_ORIGIN_Y },{ BATTERY_H, BATTERY_W} };
	get_battery_layer_str(str);
	LayerText lt_battery = { str, frame_battery_text, GAlignCenter, U_ASCII_ARIAL_12, 0 };
	P_Layer layer_battery_text = app_layer_create_text(&lt_battery);
	if (layer_battery_text != NULL)
	{
		g_app_battery_layer_id = app_window_add_layer(pwindow, layer_battery_text);
	}

}

void app_mwd_watch_time_change(enum SysEventType type, void *context)
{
    /*如果系统事件是时间更改*/
    if (type == SysEventTypeTimeChange)
    {
        /*根据窗口ID获取窗口句柄*/
        P_Window p_window = (P_Window)app_window_stack_get_window_by_id(g_app_window_id);
        if (NULL == p_window)
            return;

        P_Layer p_time_layer = app_window_get_layer_by_id(p_window, g_app_time_layer_id);
        if (NULL == p_time_layer)
            return;

		P_Layer p_date_layer = app_window_get_layer_by_id(p_window, g_app_date_layer_id);
		if (NULL == p_date_layer)
			return;

		P_Layer p_sport_layer = app_window_get_layer_by_id(p_window, g_app_sport_layer_id);
		if (NULL == p_sport_layer)
			return;

		P_Layer p_battery_layer = app_window_get_layer_by_id(p_window, g_app_battery_layer_id);
		if (NULL == p_battery_layer)
			return;

		P_Layer p_ble_layer = app_window_get_layer_by_id(p_window, g_app_ble_layer_id);
		if (NULL == p_ble_layer)
			return;

		P_Layer p_floor_layer = app_window_get_layer_by_id(p_window, g_app_floor_layer_id);
		if (NULL == p_floor_layer)
			return;

		P_Layer p_height_layer = app_window_get_layer_by_id(p_window, g_app_height_layer_id);
		if (NULL == p_height_layer)
			return;		
		
        char str[20] = "";
		char str2[20] = "";
        //Left Layer
        get_time_layer_str(str);
        app_layer_set_text_text(p_time_layer, str);

		get_date_layer_str(str);
		app_layer_set_text_text(p_date_layer, str);

		get_sport_layer_str(str,str2);
		app_layer_set_text_text(p_sport_layer, str);
		app_layer_set_text_text(p_floor_layer,str2);

		get_battery_layer_str(str);
		app_layer_set_text_text(p_battery_layer, str);
		
		get_height_layer_str(str);
		app_layer_set_text_text(p_height_layer,str);

		//蓝牙图层处理
		GBitmap bitmap_ble_con;
		GBitmap bitmap_ble_discon;
		GRect frame_ble = { { BLE_ORIGIN_X,BLE_ORIGIN_Y },{ BLE_H,BLE_W } };
		res_get_user_bitmap(RES_BITMAP_BLE_CON, &bitmap_ble_con);
		res_get_user_bitmap(RES_BITMAP_BLE_DISCON, &bitmap_ble_discon);
		LayerBitmap layer_bitmap_ble_con = { bitmap_ble_con,frame_ble,GAlignCenter };
		LayerBitmap layer_bitmap_ble_discon = { bitmap_ble_discon,frame_ble,GAlignCenter };
		enum BleStatus ble_sta;
		ble_sta = maibu_get_ble_status();
		if (ble_sta != BLE_STATUS_CONNECTED) {
			P_Layer layer_ble_new = app_layer_create_bitmap(&layer_bitmap_ble_discon);
			g_app_ble_layer_id = app_window_replace_layer(p_window, p_ble_layer, layer_ble_new);
		}
		if(ble_sta== BLE_STATUS_CONNECTED) {
			P_Layer layer_ble_new = app_layer_create_bitmap(&layer_bitmap_ble_con);
			g_app_ble_layer_id = app_window_replace_layer(p_window, p_ble_layer, layer_ble_new);
		}

		app_window_update(p_window);
    }
}

//更新蓝牙状态
void app_timer_change()
{
	/*根据窗口ID获取窗口句柄*/
	P_Window p_window = (P_Window)app_window_stack_get_window_by_id(g_app_window_id);
	if (NULL == p_window)
		return;
	P_Layer p_ble_layer = app_window_get_layer_by_id(p_window, g_app_ble_layer_id);
	if (NULL == p_ble_layer)
		return;

	GBitmap bitmap_ble_con;
	GBitmap bitmap_ble_discon;
	GRect frame_ble = { { BLE_ORIGIN_X,BLE_ORIGIN_Y },{ BLE_H,BLE_W } };
	res_get_user_bitmap(RES_BITMAP_BLE_CON, &bitmap_ble_con);
	res_get_user_bitmap(RES_BITMAP_BLE_DISCON, &bitmap_ble_discon);
	LayerBitmap layer_bitmap_ble_con = { bitmap_ble_con,frame_ble,GAlignCenter };
	LayerBitmap layer_bitmap_ble_discon = { bitmap_ble_discon,frame_ble,GAlignCenter };
	enum BleStatus ble_sta;
	ble_sta = maibu_get_ble_status();
	if (ble_sta != BLE_STATUS_CONNECTED) {
		P_Layer layer_ble_new = app_layer_create_bitmap(&layer_bitmap_ble_discon);
		g_app_ble_layer_id = app_window_replace_layer(p_window, p_ble_layer, layer_ble_new);
	}
	if(ble_sta== BLE_STATUS_CONNECTED) {
		P_Layer layer_ble_new = app_layer_create_bitmap(&layer_bitmap_ble_con);
		g_app_ble_layer_id = app_window_replace_layer(p_window, p_ble_layer, layer_ble_new);
	}

	app_window_update(p_window);
}

P_Window init_mwd_window()
{
    P_Window p_window = app_window_create();
    if (NULL == p_window) {
        return NULL;
    }

    display_bitmap(p_window);

    init_text_layer(p_window);

    /*注册一个事件通知回调，当有时间改变时，立即更新时间*/
    maibu_service_sys_event_subscribe(app_mwd_watch_time_change);

    return p_window;
}

int main()
{
    /*创建显示时间窗口*/
    P_Window p_window = init_mwd_window();
    if (p_window != NULL)
    {
        /*放入窗口栈显示*/
        g_app_window_id = app_window_stack_push(p_window);
    }
	//每隔10秒检测一次蓝牙状态
	app_timer_change_id = app_service_timer_subscribe(1000*10, app_timer_change, NULL);
}
