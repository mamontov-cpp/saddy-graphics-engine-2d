/*
** This source file is part of framepacker
**
** For the latest info, see https://github.com/paladin-t/framepacker/
**
** Copyright (C) 2015 Wang Renxin
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __FRAMEPACKER_H__
#define __FRAMEPACKER_H__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>

namespace framepacker {

struct match_path_separator { bool operator () (char ch) const { return ch == '\\' || ch == '/'; } };

std::string basename(const std::string &pathname) {
	return std::string(std::find_if(pathname.rbegin(), pathname.rend(), match_path_separator()).base(), pathname.end());
}

struct vec2 {
	vec2() : x(0), y(0) {
	}
	vec2(int _x, int _y) : x(_x), y(_y) {
	}

	vec2 operator + (const vec2 &o) const {
		return vec2(x + o.x, y + o.y);
	}
	vec2 operator - (const vec2 &o) const {
		return vec2(x - o.x, y - o.y);
	}

	bool is_zero(void) const {
		return x == 0 && y == 0;
	}

	int x;
	int y;
};

struct rect {
	rect() {
	}
	rect(int l, int t, int r, int b) {
		min = vec2(l, t);
		max = vec2(r, b);
	}

	int min_x(void) const {
		return min.x;
	}
	int min_y(void) const {
		return min.y;
	}
	int max_x(void) const {
		return max.x;
	}
	int max_y(void) const {
		return max.y;
	}

	void min_x(int v) {
		min.x = v;
		if(v > max.x)
			max.x = v;
	}
	void min_y(int v) {
		min.y = v;
		if(v > max.y)
			max.y = v;
	}
	void max_x(int v) {
		max.x = v;
		if(v < min.x)
			min.x = v;
	}
	void max_y(int v) {
		max.y = v;
		if(v < min.y)
			min.y = v;
	}

	int width(void) const {
		return max.x - min.x;
	}
	int height(void) const {
		return max.y - min.y;
	}

	void width(unsigned w) {
		max_x(min_x() + w);
	}
	void height(unsigned h) {
		max_y(min_y() + h);
	}

	int area(void) const {
		return width() * height();
	}

	vec2 left_top(void) const {
		return min;
	}
	vec2 left_bottom(void) const {
		return vec2(min.x, max.y);
	}
	vec2 right_top(void) const {
		return vec2(max.x, min.y);
	}
	vec2 right_bottom(void) const {
		return max;
	}

	vec2 size(void) const {
		return vec2(max.x - min.x, max.y - min.y);
	}

	rect add(int p) const {
		return rect(min.x + p, min.y + p, max.x + p, max.y + p);
	}

	vec2 min;
	vec2 max;
};

std::ostream &operator << (std::ostream &s, const vec2 &r) {
	s << "{" << r.x << "," << r.y << "}";

	return s;
}

std::ostream &operator << (std::ostream &s, const rect &r) {
	s << "{" << r.min << "," << r.size() << "}";

	return s;
}

struct node : public rect {
	typedef std::shared_ptr<node> ptr_type;

	node() : used(false), down(NULL), right(NULL) {
	}
	node(int x, int y, int w, int h) : used(false), down(NULL), right(NULL) {
		min_x(x);
		min_y(y);
		width(w);
		height(h);
	}
	node(const node &o) {
		min_x(o.min_x());
		min_y(o.min_y());
		max_x(o.max_x());
		max_y(o.max_y());
		used = o.used;
		down = o.down;
		right = o.right;
	}
	~node() {
		clear();
	}

	void clear(void) {
		if(down) {
			down->clear();
			down = NULL;
		}
		if(right) {
			right->clear();
			right = NULL;
		}
	}

	bool used;
	node::ptr_type down;
	node::ptr_type right;
};

template<typename T>
struct block {
	typedef T texture_type;

	block() : fit(NULL), rotated(false) {
	}
	block(const texture_type &i, const char* p, bool alpha_trim) : fit(NULL), rotated(false) {
		texture = i;
		calc_valid(alpha_trim);
		path = p;
		name = basename(p);
	}
	block(const block &o) {
		fit = o.fit;
		texture = o.texture;
		valid = o.valid;
		path = o.path;
		name = o.name;
		rotated = o.rotated;
	}
	~block() {
	}

	block &operator = (const block &o) {
		fit = o.fit;
		texture = o.texture;
		valid = o.valid;
		path = o.path;
		name = o.name;

		return *this;
	}

	rect valid_area(void) const {
		if(!rotated) return valid;

		rect ret;
		ret.min = valid.min;
		ret.max = vec2(valid.max_y(), valid.max_x());

		return ret;
	}

	void calc_valid(bool alpha_trim) {
		if(alpha_trim) {
			int minx = std::numeric_limits<int>::max();
			int miny = std::numeric_limits<int>::max();
			int maxx = std::numeric_limits<int>::min();
			int maxy = std::numeric_limits<int>::min();

			bool found = false;
			for(int j = 0; j < texture->height(); j++) {
				for(int i = 0; i < texture->width(); i++) {
					if(!texture->is_transparent(i, j)) {
						miny = j;
						found = true;

						break;
					}
				}
				if(found)
					break;
			}
			found = false;
			for(int j = texture->height() - 1; j >= 0; j--) {
				for(int i = 0; i < texture->width(); i++) {
					if(!texture->is_transparent(i, j)) {
						maxy = j;
						found = true;

						break;
					}
				}
				if(found)
					break;
			}
			found = false;
			for(int i = 0; i < texture->width(); i++) {
				for(int j = miny; j <= maxy; j++) {
					if(!texture->is_transparent(i, j)) {
						minx = i;
						found = true;

						break;
					}
				}
				if(found)
					break;
			}
			found = false;
			for(int i = texture->width() - 1; i >= 0; i--) {
				for(int j = miny; j <= maxy; j++) {
					if(!texture->is_transparent(i, j)) {
						maxx = i;
						found = true;

						break;
					}
				}
				if(found)
					break;
			}

			valid.min_x(minx);
			valid.min_y(miny);
			valid.max_x(maxx);
			valid.max_y(maxy);
		} else {
			valid.min_x(0);
			valid.min_y(0);
			valid.max_x(texture->width());
			valid.max_y(texture->height());
		}
	}

	void clear(void) {
		if(fit)
			fit->clear();
		rotated = false;
	}

	void write_meta(std::ostream &s, int &indent, int padding) const {
		indent++;
		rect frame;
		vec2 loc = fit->min;
		frame.min = loc;
		frame.max = loc + valid.size() - vec2(1, 1);
		s << get_indent(indent) << "frame : " << "\"" << frame.add(padding) << "\"" << "," << std::endl;
		s << get_indent(indent) << "offset : " << "\"" << vec2() << "\"" << "," << std::endl;
		s << get_indent(indent) << "rotated : " << "false" << "," << std::endl;
		s << get_indent(indent) << "sourceColorRect : " << "\"" << valid << "\"" << "," << std::endl;
		s << get_indent(indent) << "sourceSize : " << "\"" << vec2(texture->width(), texture->height()) << "\"" << std::endl;
		indent--;
	}

	std::string get_indent(int indent) const {
		std::string ret;
		for(int i = 0; i < indent; i++)
			ret += "\t";

		return ret;
	}

	node::ptr_type fit;
	texture_type texture;
	rect valid;
	std::string path;
	std::string name;
	bool rotated;
};

template<typename I, bool per_pixel = false, bool sqrt_area = true>
class packer {

public:
	typedef I image_type;
	typedef std::shared_ptr<image_type> texture_type;
	typedef block<texture_type> block_type;
	typedef std::string name_type;
	typedef std::pair<name_type, block_type> texture_item_type;
	typedef std::list<texture_item_type> texture_coll_type;
	typedef bool (* texture_compare_type)(const texture_item_type &, const texture_item_type &);

	static bool compare_smart(const texture_item_type &l, const texture_item_type &r) {
		return l.second.valid.width() < r.second.valid.width();
	}
	static bool compare_rand(const texture_item_type &l, const texture_item_type &r) {
		return std::rand() % 100 < 50;
	}
	static bool compare_width(const texture_item_type &l, const texture_item_type &r) {
		return l.second.valid.width() < r.second.valid.width();
	}
	static bool compare_height(const texture_item_type &l, const texture_item_type &r) {
		return l.second.valid.height() < r.second.valid.height();
	}
	static bool compare_area(const texture_item_type &l, const texture_item_type &r) {
		return l.second.valid.area() > r.second.valid.area();
	}

public:
	packer() : comparer(compare_smart), padding(1), allow_rotate(true), power_of_2(true), alpha_trim(true) {
		total_area = 0;
		indent = 0;
	}
	~packer() {
		tidy();
	}

	const block_type* get(const name_type &name) const {
		for(texture_coll_type::const_iterator it = images.begin(); it != images.end(); ++it) {
			const texture_item_type &i = *it;
			const block_type &blk = i.second;
			if(i.first == name)
				return &blk;
		}

		return NULL;
	}
	bool add(const name_type &name, const texture_type &img) {
		if(get(name))
			return false;

		images.push_back(texture_item_type(name, block_type(img, name.c_str(), alpha_trim)));
		total_area += images.back().second.valid.area();

		return true;
	}
	bool remove(const name_type &name) {
		if(!get(name))
			return false;

		for(texture_coll_type::iterator it = images.begin(); it != images.end(); ++it) {
			texture_item_type &i = *it;
			block_type &blk = i.second;
			if(i.first == name) {
				total_area -= blk.valid.area();

				images.erase(it);

				break;
			}
		}

		return true;
	}
	void clear(void) {
		tidy();

		images.clear();

		total_area = 0;
	}

	void tidy(void) {
		for(texture_coll_type::iterator it = images.begin(); it != images.end(); ++it) {
			texture_item_type &i = *it;
			block_type &blk = i.second;
			blk.clear();
		}
		if(root)
			root->clear();
	}

	void write_meta(std::ostream &s, const texture_coll_type &p, const texture_type &t, const char* n) {
		s << "{" << std::endl;
		{
			indent++;
			s << get_indent() << "frames : {" << std::endl;
			{
				indent++;
				int i = 0;
				for(texture_coll_type::const_iterator it = p.begin(); it != p.end(); ++it) {
					const block_type &blk = it->second;
					s << get_indent() << blk.name << " : {" << std::endl;
					blk.write_meta(s, indent, padding);
					if(i == p.size() - 1)
						s << get_indent() << "}" << std::endl;
					else
						s << get_indent() << "}," << std::endl;
					i++;
				}
				indent--;
			}
			s << get_indent() << "}," << std::endl;
			indent--;
		}
		{
			indent++;
			s << get_indent() << "metadata : {" << std::endl;
			{
				indent++;
				s << get_indent() << "textureName : " << n << "," << std::endl;
				s << get_indent() << "size : " << "\"" << vec2(t->width(), t->height()) << "\"" << std::endl;
				indent--;
			}
			s << get_indent() << "}" << std::endl;
			indent--;
		}
		s << "}" << std::endl;
	}

	void pack(texture_type &result, texture_coll_type &packed, texture_coll_type &failed) {
		// Step 1. Tidy.
		tidy();

		// Step 2. Sort.
		images.sort(comparer);

		// Step 3. Fit.
		if(output_texture_size.is_zero()) {
			if(sqrt_area) {
				int s = (int)(std::sqrt(total_area) + 0.5f);
				root = node::ptr_type(new node(0, 0, s, s));
			} else {
				int w = images.size() ? (images.begin()->second.valid.width() + (padding * 2)): 0;
				int h = images.size() ? (images.begin()->second.valid.height() + (padding * 2)) : 0;
				root = node::ptr_type(new node(0, 0, w, h));
			}
		} else {
			root = node::ptr_type(new node(0, 0, output_texture_size.x, output_texture_size.y));
		}
		node::ptr_type nd = NULL;
		for(texture_coll_type::iterator it = images.begin(); it != images.end(); ++it) {
			texture_item_type &i = *it;
			block_type &blk = i.second;
			if(nd = find(root, blk.valid.width() + (padding * 2), blk.valid.height() + (padding * 2)))
				blk.fit = split(nd, blk.valid.width() + (padding * 2), blk.valid.height() + (padding * 2));
			else
				blk.fit = grow(blk.valid.width() + (padding * 2), blk.valid.height() + (padding * 2));

			if(!blk.fit && allow_rotate) {
				blk.rotated = true;
				if(nd = find(root, blk.valid_area().width() + (padding * 2), blk.valid_area().height() + (padding * 2)))
					blk.fit = split(nd, blk.valid_area().width() + (padding * 2), blk.valid_area().height() + (padding * 2));
				else
					blk.fit = grow(blk.valid_area().width() + (padding * 2), blk.valid_area().height() + (padding * 2));
			}

			if(blk.fit)
				packed.push_back(texture_item_type(i.first, block_type(blk)));
			else
				failed.push_back(texture_item_type(i.first, block_type(blk)));
		}

		// Step 4. Draw.
		vec2 os(root->width(), root->height());
		if(power_of_2) {
			os.x = static_cast<int>(std::pow(2, std::ceil(std::log(static_cast<double>(os.x)) / std::log(2.0))));
			os.y = static_cast<int>(std::pow(2, std::ceil(std::log(static_cast<double>(os.y)) / std::log(2.0))));
		}
		result->resize(os.x, os.y);
		for(texture_coll_type::iterator it = packed.begin(); it != packed.end(); ++it) {
			texture_item_type &i = *it;
			block_type &blk = i.second;
			if(blk.rotated) {
				for(int j = 0; j < blk.valid.height(); j++) {
					for(int i = 0; i < blk.valid.width(); i++) {
						int x = i + blk.fit->min_x() + padding;
						int y = j + blk.fit->min_y() + padding;
						result->pixel(y, x, blk.texture->pixel(i + blk.valid.min_x(), j + blk.valid.min_y()));
					}
				}
			} else {
				if(per_pixel) {
					for(int j = 0; j < blk.valid.height(); j++) {
						for(int i = 0; i < blk.valid.width(); i++) {
							int x = i + blk.fit->min_x() + padding;
							int y = j + blk.fit->min_y() + padding;
							result->pixel(x, y, blk.texture->pixel(i + blk.valid.min_x(), j + blk.valid.min_y()));
						}
					}
				} else {
					result->copy_from(
						*blk.texture.get(),
						blk.valid.min_x(), blk.texture->height() - blk.valid.min_y() - blk.valid.height(),
						blk.valid.width(), blk.valid.height(),
						blk.fit->min_x() + padding, blk.fit->min_y() + padding
					);
				}
			}
		}
	}

private:
	node::ptr_type find(node::ptr_type root, int w, int h) {
		if(root->used) {
			node::ptr_type r = find(root->right, w, h);
			if(!r) r = find(root->down, w, h);

			return r;
		} else if(w <= root->width() && h <= root->height()) {
			return root;
		} else {
			return NULL;
		}
	}

	node::ptr_type split(node::ptr_type nd, int w, int h) {
		nd->used = true;
		nd->down = node::ptr_type(new node(nd->min_x(), nd->min_y() + h, nd->width(), nd->height() - h));
		nd->right = node::ptr_type(new node(nd->min_x() + w, nd->min_y(), nd->width() - w, h));

		return nd;
	}

	node::ptr_type grow(int w, int h) {
		bool can_grow_down = w <= root->width();
		bool can_grow_right = h <= root->height();

		bool should_grow_right = can_grow_right && (root->height() >= (root->width() + w));
		bool should_grow_down  = can_grow_down && (root->width() >= (root->height() + h));

		if(should_grow_right)
			return grow_right(w, h);
		else if(should_grow_down)
			return grow_down(w, h);
		else if(can_grow_right)
			return grow_right(w, h);
		else if(can_grow_down)
			return grow_down(w, h);
		else
			return NULL;
	}

	node::ptr_type grow_right(int w, int h) {
		node* r = new node(0, 0, root->width() + w, root->height());
		r->used = true;
		r->down = root;
		r->right = node::ptr_type(new node(root->width(), 0, w, root->height()));
		root = node::ptr_type(r);

		node::ptr_type node = NULL;
		if(node = find(root, w, h))
			return split(node, w, h);
		else
			return NULL;
	}
	node::ptr_type grow_down(int w, int h) {
		node* r = new node(0, 0, root->width(), root->height() + h);
		r->used = true;
		r->down = node::ptr_type(new node(0, root->height(), root->width(), h));
		r->right = root;
		root = node::ptr_type(r);

		node::ptr_type node = NULL;
		if(node = find(root, w, h))
			return split(node, w, h);
		else
			return NULL;
	}

	std::string get_indent(void) const {
		std::string ret;
		for(int i = 0; i < indent; i++)
			ret += "\t";

		return ret;
	}

public:
	texture_compare_type comparer;

	int padding;
	bool allow_rotate;
	bool power_of_2;
	bool alpha_trim;
	vec2 output_texture_size;

private:
	texture_coll_type images;
	node::ptr_type root;
	double total_area;

	int indent;

};

}

#endif // __FRAMEPACKER_H__
