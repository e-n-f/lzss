#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <map>
#include <string>

struct node {
	// Each node will either have a >=0 code
	// or will have left and right pointers
	// to children

	std::shared_ptr<node> left;
	std::shared_ptr<node> right;
	int code;
	size_t weight;
};

void walk(std::shared_ptr<node> n, std::string prefix) {
	if (n->code >= 0) {
		printf("%c %s\n", n->code, prefix.c_str());
	} else {
		walk(n->left, prefix + "0");
		walk(n->right, prefix + "1");
	}
}

int main(int argc, char **argv) {
	std::string text;
	int c;

	while ((c = getchar()) != EOF) {
		text.push_back(c);
	}

	// Calculate frequency for each token

	size_t freq[256];
	for (size_t i = 0; i < 256; i++) {
		freq[i] = 0;
	}

	for (size_t i = 0; i < text.size(); i++) {
		freq[(unsigned char) text[i]]++;
	}

	// Set up a tree node for each token
	// and put them into a pool

	std::multimap<size_t, std::shared_ptr<node>> remaining_nodes;
	for (size_t i = 0; i < 256; i++) {
		if (freq[i] > 0) {
			std::shared_ptr<node> n = std::make_shared<node>();
			n->code = i;
			n->weight = freq[i];

			remaining_nodes.insert(std::pair<size_t, std::shared_ptr<node>>(n->weight, n));
		}
	}

	// Build up a tree of frequencies

	while (remaining_nodes.size() > 1) {
		// Pull off the two least-frequent nodes and merge them together
		// under a shared parent

		auto i = remaining_nodes.begin();

		std::shared_ptr<node> n1 = i->second;
		++i;

		std::shared_ptr<node> n2 = i->second;
		++i;

		remaining_nodes.erase(remaining_nodes.begin(), i);

		std::shared_ptr<node> parent = std::make_shared<node>();
		parent->weight = n1->weight + n2->weight;
		parent->code = -1;
		parent->left = n1;
		parent->right = n2;

		remaining_nodes.insert(std::pair<size_t, std::shared_ptr<node>>(parent->weight, parent));
	}

	// Walk the tree

	walk(remaining_nodes.begin()->second, "");
}
