#include "fontnode.h"

int TestFontNode::Type=2;

TestFontNode::TestFontNode(sad::TMFont * fnt)
{
	m_type=TestFontNode::Type;
    m_fnt=fnt;
}

TestFontNode::~TestFontNode()
{

}

void TestFontNode::render()
{
	hst::string test("Test");
	hRectF rect(pointf(-0.06,-0.06),pointf(0.06,0.06));
	m_fnt->render(test,rect,0.5);
}