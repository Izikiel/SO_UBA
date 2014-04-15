class Proc
{
    public:
        Proc(): start(0), deadline(1000 * 10000), pid(-1) {};
        Proc(int new_start, int new_deadline, int new_pid):
            start(new_start), deadline(new_deadline), pid(new_pid) {};
        ~Proc() {};
        int priority() const {return deadline - start;};
        bool operator >(Proc b) const
        {
            return priority() < b.priority();
        }
        bool operator <(Proc b) const
        {
            return priority() > b.priority();
        }
        bool operator ==(Proc b) const
        {
            return (get_start() == b.get_start()) &&
                   (get_deadline() == b.get_deadline()) &&
                   (get_pid() == b.get_pid());
        }
        int get_start() const {return start;}
        int get_deadline() const {return deadline;}
        int get_pid() const {return pid;}
        void tick() {start += 1;}
    private:
        int start, deadline, pid;
};