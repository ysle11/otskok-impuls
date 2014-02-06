#property indicator_separate_window
#property indicator_buffers 5
#property indicator_color1 LimeGreen
#property indicator_color2 FireBrick
#property indicator_color3 Green
#property indicator_color4 Red
#property indicator_color5 Black
#property indicator_width1 3
#property indicator_width2 3
#property indicator_width3 1
#property indicator_width4 1

extern string separator1 = "*** OSMA Settings ***";
extern int fastEMA = 12;
extern int slowEMA = 26;
extern int signal = 9;
extern string separator2 = "*** Indicator Settings ***";
extern double positiveSensitivity = 0.0001;
extern double negativeSensitivity = -0.0001;
extern double historyBarsCount = 0.0;
extern bool drawDivergenceLines = TRUE;
extern bool displayAlert = TRUE;
double g_ibuf_136[];
double g_ibuf_140[];
double g_ibuf_144[];
double g_ibuf_148[];
double g_ibuf_152[];
datetime g_time_156;

int init() {
   SetIndexStyle(0, DRAW_HISTOGRAM, STYLE_SOLID);
   SetIndexStyle(1, DRAW_HISTOGRAM, STYLE_SOLID);
   SetIndexStyle(2, DRAW_ARROW);
   SetIndexStyle(3, DRAW_ARROW);
   SetIndexStyle(4, DRAW_NONE);
   SetIndexBuffer(0, g_ibuf_136);
   SetIndexBuffer(1, g_ibuf_140);
   SetIndexBuffer(2, g_ibuf_144);
   SetIndexBuffer(3, g_ibuf_148);
   SetIndexBuffer(4, g_ibuf_152);
   SetIndexArrow(2, 233);
   SetIndexArrow(3, 234);
   IndicatorDigits(Digits + 2);
   IndicatorShortName("FX5_Divergence_v2.0(" + fastEMA + "," + slowEMA + "," + signal + ")");
   return (0);
}

int deinit() {
   string l_name_0;
   for (int li_8 = ObjectsTotal() - 1; li_8 >= 0; li_8--) {
      l_name_0 = ObjectName(li_8);
      if (StringSubstr(l_name_0, 0, 14) == "DivergenceLine") ObjectDelete(l_name_0);
   }
   return (0);
}

int start() {
   int l_ind_counted_0 = IndicatorCounted();
   if (l_ind_counted_0 < 0) l_ind_counted_0 = 0;
   CalculateIndicator(l_ind_counted_0);
   return (0);
}

void CalculateIndicator(int ai_0) {
   for (int li_4 = Bars - ai_0; li_4 >= 0; li_4--) {
      CalculateOsMA(li_4);
      CatchBullishDivergence(li_4 + 2);
      CatchBearishDivergence(li_4 + 2);
   }
}

void CalculateOsMA(int ai_0) {
   g_ibuf_152[ai_0] = iOsMA(NULL, 0, fastEMA, slowEMA, signal, PRICE_CLOSE, ai_0);
   if (g_ibuf_152[ai_0] > 0.0) {
      g_ibuf_136[ai_0] = g_ibuf_152[ai_0];
      g_ibuf_140[ai_0] = 0;
      return;
   }
   if (g_ibuf_152[ai_0] < 0.0) {
      g_ibuf_140[ai_0] = g_ibuf_152[ai_0];
      g_ibuf_136[ai_0] = 0;
      return;
   }
   g_ibuf_136[ai_0] = 0;
   g_ibuf_140[ai_0] = 0;
}

void CatchBullishDivergence(int ai_0) {
   int li_4;
   int li_8;
   if (IsIndicatorTrough(ai_0) != 0) {
      li_4 = ai_0;
      li_8 = GetIndicatorLastTrough(ai_0);
      if (g_ibuf_152[li_4] > g_ibuf_152[li_8] && Low[li_4] < Low[li_8]) {
         g_ibuf_144[li_4] = g_ibuf_152[li_4];
         if (drawDivergenceLines == TRUE) {
            DrawPriceTrendLine(Time[li_4], Time[li_8], Low[li_4], Low[li_8], Green, STYLE_SOLID);
            DrawIndicatorTrendLine(Time[li_4], Time[li_8], g_ibuf_152[li_4], g_ibuf_152[li_8], Green, STYLE_SOLID);
         }
         if (displayAlert == TRUE) DisplayAlert("Classical bullish divergence on: ", li_4);
      }
      if (g_ibuf_152[li_4] < g_ibuf_152[li_8] && Low[li_4] > Low[li_8]) {
         g_ibuf_144[li_4] = g_ibuf_152[li_4];
         if (drawDivergenceLines == TRUE) {
            DrawPriceTrendLine(Time[li_4], Time[li_8], Low[li_4], Low[li_8], Green, STYLE_DOT);
            DrawIndicatorTrendLine(Time[li_4], Time[li_8], g_ibuf_152[li_4], g_ibuf_152[li_8], Green, STYLE_DOT);
         }
         if (displayAlert == TRUE) DisplayAlert("Reverse bullish divergence on: ", li_4);
      }
   }
}

void CatchBearishDivergence(int ai_0) {
   int li_4;
   int li_8;
   if (IsIndicatorPeak(ai_0) != 0) {
      li_4 = ai_0;
      li_8 = GetIndicatorLastPeak(ai_0);
      if (g_ibuf_152[li_4] < g_ibuf_152[li_8] && High[li_4] > High[li_8]) {
         g_ibuf_148[li_4] = g_ibuf_152[li_4];
         if (drawDivergenceLines == TRUE) {
            DrawPriceTrendLine(Time[li_4], Time[li_8], High[li_4], High[li_8], Red, STYLE_SOLID);
            DrawIndicatorTrendLine(Time[li_4], Time[li_8], g_ibuf_152[li_4], g_ibuf_152[li_8], Red, STYLE_SOLID);
         }
         if (displayAlert == TRUE) DisplayAlert("Ray maybe reverse Bullish divergence on: ", li_4);
      }
      if (g_ibuf_152[li_4] > g_ibuf_152[li_8] && High[li_4] < High[li_8]) {
         g_ibuf_148[li_4] = g_ibuf_152[li_4];
         if (drawDivergenceLines == TRUE) {
            DrawPriceTrendLine(Time[li_4], Time[li_8], High[li_4], High[li_8], Red, STYLE_SOLID);
            DrawIndicatorTrendLine(Time[li_4], Time[li_8], g_ibuf_152[li_4], g_ibuf_152[li_8], Red, STYLE_SOLID);
         }
         if (displayAlert == TRUE) DisplayAlert("Divergence Ray Bullish Long should continue  on: ", li_4);
      }
   }
}

int IsIndicatorPeak(int ai_0) {
   if (g_ibuf_152[ai_0] > positiveSensitivity + 0.0 && g_ibuf_152[ai_0] > g_ibuf_152[ai_0 + 1] && g_ibuf_152[ai_0] > g_ibuf_152[ai_0 - 1]) {
      for (int li_4 = ai_0 + 1; li_4 < Bars; li_4++) {
         if (g_ibuf_152[li_4] < 0.0) return (1);
         if (g_ibuf_152[li_4] > g_ibuf_152[ai_0]) break;
      }
   }
   return (0);
}

int IsIndicatorTrough(int ai_0) {
   if (g_ibuf_152[ai_0] < negativeSensitivity + 0.0 && g_ibuf_152[ai_0] < g_ibuf_152[ai_0 + 1] && g_ibuf_152[ai_0] < g_ibuf_152[ai_0 - 1]) {
      for (int li_4 = ai_0 + 1; li_4 < Bars; li_4++) {
         if (g_ibuf_152[li_4] > 0.0) return (1);
         if (g_ibuf_152[li_4] < g_ibuf_152[ai_0]) break;
      }
   }
   return (0);
}

int GetIndicatorLastPeak(int ai_0) {
   bool li_4 = FALSE;
   for (int li_ret_8 = ai_0; li_ret_8 < Bars; li_ret_8++) {
      if (g_ibuf_152[li_ret_8] > 0.0 && li_4 == FALSE) continue;
      if (g_ibuf_152[li_ret_8] < 0.0) li_4 = TRUE;
      else {
         if (g_ibuf_152[li_ret_8] >= positiveSensitivity)
            if (g_ibuf_152[li_ret_8] > g_ibuf_152[li_ret_8 + 1] && g_ibuf_152[li_ret_8] > g_ibuf_152[li_ret_8 - 1]) return (li_ret_8);
      }
   }
   return (-1);
}

int GetIndicatorLastTrough(int ai_0) {
   bool li_4 = FALSE;
   for (int li_ret_8 = ai_0; li_ret_8 < Bars; li_ret_8++) {
      if (g_ibuf_152[li_ret_8] < 0.0 && li_4 == FALSE) continue;
      if (g_ibuf_152[li_ret_8] > 0.0) li_4 = TRUE;
      else {
         if (g_ibuf_152[li_ret_8] <= negativeSensitivity)
            if (g_ibuf_152[li_ret_8] < g_ibuf_152[li_ret_8 + 1] && g_ibuf_152[li_ret_8] < g_ibuf_152[li_ret_8 - 1]) return (li_ret_8);
      }
   }
   return (-1);
}

void DisplayAlert(string as_0, int ai_8) {
   if (ai_8 <= 2 && Time[ai_8] != g_time_156) {
      g_time_156 = Time[ai_8];
      Alert(as_0, Symbol(), " , ", Period(), " minutes chart");
   }
}

void DrawPriceTrendLine(int a_datetime_0, int a_datetime_4, double a_price_8, double a_price_16, color a_color_24, double a_style_28) {
   string l_name_36 = "DivergenceLine2.0# " + DoubleToStr(a_datetime_0, 0);
   ObjectDelete(l_name_36);
   ObjectCreate(l_name_36, OBJ_TREND, 0, a_datetime_0, a_price_8, a_datetime_4, a_price_16, 0, 0);
   ObjectSet(l_name_36, OBJPROP_RAY, FALSE);
   ObjectSet(l_name_36, OBJPROP_COLOR, a_color_24);
   ObjectSet(l_name_36, OBJPROP_STYLE, a_style_28);
}

void DrawIndicatorTrendLine(int a_datetime_0, int a_datetime_4, double a_price_8, double a_price_16, color a_color_24, double a_style_28) {
   string l_name_36;
   int l_window_44 = WindowFind("FX5_Divergence_v2.0(" + fastEMA + "," + slowEMA + "," + signal + ")");
   if (l_window_44 >= 0) {
      l_name_36 = "DivergenceLine2.0$# " + DoubleToStr(a_datetime_0, 0);
      ObjectDelete(l_name_36);
      ObjectCreate(l_name_36, OBJ_TREND, l_window_44, a_datetime_0, a_price_8, a_datetime_4, a_price_16, 0, 0);
      ObjectSet(l_name_36, OBJPROP_RAY, FALSE);
      ObjectSet(l_name_36, OBJPROP_COLOR, a_color_24);
      ObjectSet(l_name_36, OBJPROP_STYLE, a_style_28);
   }
}