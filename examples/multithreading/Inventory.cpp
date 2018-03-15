#include inventory.h

Inventory::Inventory(){
	items = new Item::Item[10][20];
}

Inventory::~Inventory(){
	
}

Inventory::Item::item getByIndex(int i, int j){
	if(this.items[i][j] != null)
		return this.items[i][j];
	return null;
}

Inventory::bool isFull(){
	bool isFull = true;
	for(int i = 0; i < this.items->size(); i++){
			for(int j =0; j < this.items[i]->size(); j++){
				if(items[i][j] == null){
					isFull = false;
					return isFull;	
				}
			}
			
		}
	return isFull;
}

Inventory::void addItem(Item::Item item){
	if(this.isFull() == false){
		for(int i = 0; i < this.items->size(); i++){
			for(int j =0; j < this.items[i]->size(); j++){
				if(items[i][j] == null){
					items[i][j] = item;	
				}
			}
			break;
		}
	}
}

Inventory::void replaceItem(int i1, int j1, int i2, int j2){
	Item::Item temp; 
	if(items[i2][j2] != null){
		temp = items[i1][j1];
		items[i1][j1] = items[i2][j2];
		items[i2][j2] = temp;
		temp = null;
	}
}


