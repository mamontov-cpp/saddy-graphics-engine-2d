#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "label.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



struct LabelTest : tpunit::TestFixture
{
 public:
   LabelTest() : tpunit::TestFixture(
       TEST(LabelTest::testFormatTextLineZeroWidth),
       TEST(LabelTest::testFormatTextLineLesserThanLimit),
       TEST(LabelTest::testFormatTextLineOverflowVisible),
       TEST(LabelTest::testFormatTextLineOverflowHidden),
       TEST(LabelTest::testFormatTextLineEllipsis1),
       TEST(LabelTest::testFormatTextLineEllipsis2),
       TEST(LabelTest::testFormatTextLineEllipsis3),
       TEST(LabelTest::testFormatTextLineEllipsisBegin),
       TEST(LabelTest::testFormatTextLineEllipsisEnd),
       TEST(LabelTest::testFormatTextLineEllipsisMiddle),
       TEST(LabelTest::testMakeRenderingString),
       TEST(LabelTest::testMakeRenderingStringMultiline)
   ) {}
   
   
   void testFormatTextLineZeroWidth()
   {
      sad::String result;
      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 0, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");
   }

   void testFormatTextLineLesserThanLimit()
   {
      sad::String result;
      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");
   }

   void testFormatTextLineOverflowVisible()
   {
      sad::String result;
      result = sad::Label::formatTextLine("text", 1, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 2, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text", 3, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");

      result = sad::Label::formatTextLine("text string", 4, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text string");
      
      result = sad::Label::formatTextLine("text     ", 4, sad::Label::OverflowStrategy::LOS_VISIBLE, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text     ");
   }

   void testFormatTextLineOverflowHidden()
   {
      sad::String result;
      result = sad::Label::formatTextLine("text", 1, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "t");

      result = sad::Label::formatTextLine("text", 2, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "te");

      result = sad::Label::formatTextLine("text", 3, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "tex");

      result = sad::Label::formatTextLine("text string", 4, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text");
      
      result = sad::Label::formatTextLine("text     ", 5, sad::Label::OverflowStrategy::LOS_HIDDEN, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "text ");
   }

   void testFormatTextLineEllipsis1()
   {
      sad::String result;
      result = sad::Label::formatTextLine("text", 1, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == ".");

      result = sad::Label::formatTextLine("text", 1, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == ".");

      result = sad::Label::formatTextLine("text", 1, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == ".");
   }

   void testFormatTextLineEllipsis2()
   {
      sad::String result;
      result = sad::Label::formatTextLine("text", 2, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "..");

      result = sad::Label::formatTextLine("text", 2, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "..");

      result = sad::Label::formatTextLine("text", 2, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "..");
   }

   void testFormatTextLineEllipsis3()
   {
      sad::String result;
      result = sad::Label::formatTextLine("text", 3, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "...");

      result = sad::Label::formatTextLine("text", 3, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "...");

      result = sad::Label::formatTextLine("text", 3, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "...");
   }
  
   void testFormatTextLineEllipsisBegin()
   {
      sad::String result;
      result = sad::Label::formatTextLine("texti", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "...i");

      result = sad::Label::formatTextLine("text ", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "...");

      result = sad::Label::formatTextLine("      ", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "");

      result = sad::Label::formatTextLine("tex      ", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "...");

      result = sad::Label::formatTextLine("tex    tt", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "...tt");

      result = sad::Label::formatTextLine("   tbctext", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_BEGIN);
      ASSERT_TRUE( result == "...ext");       
   }


   void testFormatTextLineEllipsisEnd()
   {
      sad::String result;
      result = sad::Label::formatTextLine("texti", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "t...");

      result = sad::Label::formatTextLine("  texti", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "...");

      result = sad::Label::formatTextLine("      ", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "");

      result = sad::Label::formatTextLine("   text", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "...");

      result = sad::Label::formatTextLine("kv text", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "kv...");

      result = sad::Label::formatTextLine("tbctext", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_END);
      ASSERT_TRUE( result == "tbc...");
   }

   void testFormatTextLineEllipsisMiddle()
   {
      sad::String result;
      result = sad::Label::formatTextLine("textt", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "t...");

      result = sad::Label::formatTextLine("text  ", 5, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "t...");

      result = sad::Label::formatTextLine("text2  ", 5, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "t...");

      result = sad::Label::formatTextLine("          ", 4, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "");

      result = sad::Label::formatTextLine("t        t", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "t...t");

      result = sad::Label::formatTextLine("tttttttttt", 6, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "t...tt");

      result = sad::Label::formatTextLine("tttttttttt", 7, sad::Label::OverflowStrategy::LOS_ELLIPSIS, sad::Label::TextEllipsisPosition::LTEP_MIDDLE);
      ASSERT_TRUE( result == "tt...tt");       
   }

   void testMakeRenderingString()
   {
      sad::String result;
      result = sad::Label::makeRenderingString(
          "t", 
          4, 
          sad::Label::OverflowStrategy::LOS_ELLIPSIS, 
          sad::Label::BreakText::LBT_NORMAL, 
          sad::Label::TextEllipsisPosition::LTEP_MIDDLE
      );
      ASSERT_TRUE( result == "t");

      result = sad::Label::makeRenderingString(
          "testing testing testing", 
          6, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_NORMAL, 
          sad::Label::TextEllipsisPosition::LTEP_MIDDLE
      );
      ASSERT_TRUE( result == "testing testing testing");

      result = sad::Label::makeRenderingString(
          "testing testing testing", 
          6, 
          sad::Label::OverflowStrategy::LOS_HIDDEN, 
          sad::Label::BreakText::LBT_NORMAL, 
          sad::Label::TextEllipsisPosition::LTEP_MIDDLE
      );
      ASSERT_TRUE( result == "testin");


      result = sad::Label::makeRenderingString(
          "testing testing testing", 
          6, 
          sad::Label::OverflowStrategy::LOS_ELLIPSIS, 
          sad::Label::BreakText::LBT_NORMAL, 
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result == "tes...");

      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting", 
          7, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result == "testing\ntesting\ntesting");

      result = sad::Label::makeRenderingString(
          "testing testing testing", 
          7, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result == "testing\ntesting\ntesting");

      result = sad::Label::makeRenderingString(
          "testing testing testing", 
          6, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result == "testing\ntesting\ntesting");

      result = sad::Label::makeRenderingString(
          "testing testing testing", 
          6, 
          sad::Label::OverflowStrategy::LOS_HIDDEN, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result == "testin\ntestin\ntestin");

      result = sad::Label::makeRenderingString(
          "testing testing testing", 
          6, 
          sad::Label::OverflowStrategy::LOS_ELLIPSIS, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result == "tes...\ntes...\ntes...");
   }

   void testMakeRenderingStringMultiline()
   {
       sad::String result;
       result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          0,
          sad::Label::OverflowStrategy::LOS_VISIBLE,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "testing\ntesting\ntesting");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          3,
          sad::Label::OverflowStrategy::LOS_VISIBLE,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "testing\ntesting\ntesting");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          3,
          sad::Label::OverflowStrategy::LOS_VISIBLE,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "testing\ntesting\ntesting\ntesting");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          3,
          sad::Label::OverflowStrategy::LOS_HIDDEN,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "testing\ntesting\ntesting");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          1,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "...");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          1,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result ==  "...");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          1,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_MIDDLE
      );
      ASSERT_TRUE( result ==  "...");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          2,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "...\ntesting");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          2,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result ==  "testing\n...");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          2,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_MIDDLE
      );
      ASSERT_TRUE( result ==  "testing\n...");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          1,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "...");
      result = sad::Label::makeRenderingString(
          "testing\ntesting\ntesting\ntesting", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          1,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result ==  "...");
      result = sad::Label::makeRenderingString(
          "testing1\ntesting2\ntesting3\ntesting4", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          3,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_BEGIN
      );
      ASSERT_TRUE( result ==  "...\ntesting3\ntesting4");
      result = sad::Label::makeRenderingString(
          "testing1\ntesting2\ntesting3\ntesting4", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          3,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_END
      );
      ASSERT_TRUE( result ==  "testing1\ntesting2\n...");
      result = sad::Label::makeRenderingString(
          "testing1\ntesting2\ntesting3\ntesting4", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          3,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_MIDDLE
      );
      ASSERT_TRUE( result ==  "testing1\n...\ntesting4");
      result = sad::Label::makeRenderingString(
          "testing1\ntesting2\ntesting3\ntesting4\ntesting5", 
          0, 
          sad::Label::OverflowStrategy::LOS_VISIBLE, 
          sad::Label::BreakText::LBT_BREAK_WORD, 
          sad::Label::TextEllipsisPosition::LTEP_END,
          4,
          sad::Label::OverflowStrategy::LOS_ELLIPSIS,
          sad::Label::TextEllipsisPosition::LTEP_MIDDLE
      );
      ASSERT_TRUE( result ==  "testing1\ntesting2\n...\ntesting5");
   }


} _label_test;

