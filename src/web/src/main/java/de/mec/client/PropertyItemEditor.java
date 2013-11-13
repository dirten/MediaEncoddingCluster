/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.Editor;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.sencha.gxt.data.shared.Converter;
import com.sencha.gxt.widget.core.client.form.*;
import de.mec.client.model.tools.ObjectConverter;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public abstract class PropertyItemEditor extends Composite implements Editor<PropertyItem> {
    //public abstract void setLabel(final String l);

    private static final Logger LOG = Logger.getLogger(PropertyItemEditor.class.getName());
    private FieldLabel label;
    private String _key;
    private Grid grid = new Grid(1, 2);

    public String getKey() {
        return _component.getName();
    }

    public void setKey(String key) {
        _component.setName(key);
    }
    
    private Field _component;
    @Path("val")
    public ConverterEditorAdapter<String, Object, Field<Object>> converter;

    public PropertyItemEditor() {
        initWidget(grid);
        label = new FieldLabel();
        grid.setWidget(0, 0, label);
    }

    public PropertyItemEditor(Field component, Converter c) {
        this();
        setComponent(component);
        setConverter(c);
        //_component = component;
        

    }

    protected final void setComponent(Field field) {
        _component = field;
        _component.setWidth(400);
        grid.setWidget(0, 1, _component);
    }

    protected final void setConverter(Converter c) {
        if (c != null) {
            converter = new ConverterEditorAdapter(_component, c);
        } else {
            converter = new ConverterEditorAdapter(_component, new ObjectConverter<String>());
        }
    }

    public void setLabel(final String l) {
        label.setText(l);
    }

    public HandlerRegistration addValueChangeHandler(ValueChangeHandler<String> handler) {
        return _component.addHandler(handler, ValueChangeEvent.getType());
    }
}
