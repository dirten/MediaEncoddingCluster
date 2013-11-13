/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.event.logical.shared.SelectionEvent;
import com.google.gwt.event.logical.shared.SelectionHandler;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.sencha.gxt.cell.core.client.form.ComboBoxCell;
import com.sencha.gxt.data.shared.Converter;
import com.sencha.gxt.data.shared.LabelProvider;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.data.shared.ModelKeyProvider;
import com.sencha.gxt.data.shared.PropertyAccess;
import com.sencha.gxt.data.shared.Store;
import com.sencha.gxt.widget.core.client.form.ComboBox;
import de.mec.client.PropertyItemEditor;
import de.mec.client.editor.guimodel.Item;
import java.util.List;

/**
 *
 * @author jan.hoelscher
 */
public class ComboboxItemEditor extends PropertyItemEditor {
    class ComboboxChangeEvent extends ValueChangeEvent<String>{
        public ComboboxChangeEvent(String field){
            super(field);
        }
    }
    class ItemIdConverter implements Converter<String, Item> {

        private final Store<Item> store;

        public ItemIdConverter(Store<Item> store) {
            this.store = store;
        }

        @Override
        public String convertFieldValue(Item object) {
            return object.getValue();
        }

        @Override
        public Item convertModelValue(String object) {
            return store.findModelWithKey(object);
        }
    }

    public interface ItemProperties extends PropertyAccess<Item> {

        ModelKeyProvider<Item> value();

        LabelProvider<Item> key();
    }
    
        ItemProperties p = GWT.create(ItemProperties.class);
        ListStore<Item> store = new ListStore<Item>(p.value());
        ComboBox<Item> box = new ComboBox<Item>(store, p.key());

    public ComboboxItemEditor() {
        super();
        box.setTriggerAction(ComboBoxCell.TriggerAction.ALL);
        box.setForceSelection(true);
        setComponent(box);
        setConverter(new ItemIdConverter(store));
        
    }

    public void setItems(List<Item> items) {
        store.addAll(items);
    }
    
    @Override
    public HandlerRegistration addValueChangeHandler(final ValueChangeHandler<String> handler) {
        return box.addSelectionHandler(new SelectionHandler<Item>() {
            public void onSelection(SelectionEvent<Item> event) {
                handler.onValueChange(new ComboboxChangeEvent(event.getSelectedItem().getValue()));
            }
        });
    }
}
