class ThereIsAGAY : public std::exception {
public:
    const char *what() const {
        return "FeiYuWan is Gay";
    }
}

class PObject {
public:
    template<class Type, class... Parameter>
        requires std::is_base_of_v<PComponent, Type>
    void RegisterComponent(PString Id, Parameter... parameter) {
        if (_list.find(Id) != _list.end()) {
            throw ThereIsAGAY;
        }
        auto component = new Type(this, parameter);
        _list.insert({ component->GetID(), component });
        _list[Id]->OnPropertyRegistering();
    }
    template<class Type>
    Type* GetComponent(PString Id) {
        return static_cast<Type>(_list[Id]);
    }

public:
    bool HasComponent() {

    }

public:
    template<class Type>
    void RegisterProperty(Type* Pointer) {
        _propertyMap.insert({ Id, reinterpret_cast<void*>(Pointer) });
    }
    template<class Type>
    Type* GetProperty(PString Id) {
        return static_cast<Type*>(_property[Id]);
    }

private:
    std::unordered_map<PString, PComponent*> _list;
    std::unordered_map<PString, void*> _propertyMap;
}

class PComponent {
public:
    PComponent(PObject* Parent) {
        _parent = Parent;
    }

public:
    virtual void OnPropertyRegistering() {

    }
    virtual PString GetID() const = 0;

protected:
    PObject *_parent;
};
class PBoundComponent : public PComponent {
public:
    PString GetID() const {
        return "Bound";
    }

protected:
    PRect _bound;

private:
    friend class PMovementComponent;
};

class PMovementComponent : public PBoundCompnent {
public:
    static PMovementComponent* MakeFromBound(PBoundComponent *Component) {
        return new PMovementComponent(Component);
    }

private:
    PMovementComponent(PBoundCompnent *Component) : _boundComponent(Component) {

    }

protected:
    PBoundComponent *_boundComponent;
    PVec2D _velocity;
};