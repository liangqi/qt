#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <QSGItem>
#include <QTime>
#include <QVector>

class ParticleAffector;
class ParticleEmitter;
class Particle;
class ParticleData;


struct EmitterData{
    int size;
    int start;
    int nextIdx;
    QHash<Particle*, int> particleOffsets;
};

class ParticleSystem : public QSGItem
{
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QDeclarativeListProperty<ParticleAffector> affectors READ affectors)
    Q_PROPERTY(QDeclarativeListProperty<ParticleEmitter> emitters READ emitters)
    Q_PROPERTY(QDeclarativeListProperty<Particle> particles READ particles)
    Q_CLASSINFO("DefaultProperty", "particles")

public:
    explicit ParticleSystem(QSGItem *parent = 0);

bool isRunning() const
{
    return m_running;
}

QDeclarativeListProperty<ParticleEmitter> emitters();

QDeclarativeListProperty<Particle> particles()
{
    return QDeclarativeListProperty<Particle>(this, m_particles);
}

QDeclarativeListProperty<ParticleAffector> affectors()
{
    return QDeclarativeListProperty<ParticleAffector>(this, m_affectors);
}
signals:

void runningChanged(bool arg);

public slots:
void pleaseUpdate(){if(this)update();}//XXX
void registerEmitter(ParticleEmitter* emitter);
void reset();
void prepareNextFrame();
void setRunning(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit runningChanged(arg);
        if(arg)
            update();
    }
}

void emitParticle(ParticleData* p);
ParticleData* newDatum(ParticleEmitter* e, Particle* p);

protected:
    Node *updatePaintNode(Node *, UpdatePaintNodeData *);

private slots:
    void countChanged();
private:
    void buildParticleNodes();
    int m_particle_count;
    bool m_running;
    bool m_do_reset;
    Node* m_node;
    QTime m_timestamp;
    QList<ParticleEmitter*> m_emitters;
    QList<ParticleAffector*> m_affectors;
    QList<Particle*> m_particles;
    QVector<ParticleData*> data;
    QList<EmitterData*> m_emitterData;//size, start
};

//TODO: Clean up all this into ParticleData

struct ParticleVertex {
    float x;
    float y;
    float t;
    float size;
    float endSize;
    float dt;
    float sx;
    float sy;
    float ax;
    float ay;
};

class ParticleData{
public:
    ParticleData();

    ParticleVertex pv;

    //Convenience functions for working backwards, because parameters are from the start of particle life
    //If setting multiple parameters at once, doing the conversion yourself will be faster.

    //sets the x accleration without affecting the instantaneous x velocity or position
    void setInstantaneousAX(qreal ax);
    //sets the x velocity without affecting the instantaneous x postion
    void setInstantaneousSX(qreal vx);
    //sets the instantaneous x postion
    void setInstantaneousX(qreal x);
    //sets the y accleration without affecting the instantaneous y velocity or position
    void setInstantaneousAY(qreal ay);
    //sets the y velocity without affecting the instantaneous y postion
    void setInstantaneousSY(qreal vy);
    //sets the instantaneous Y postion
    void setInstantaneousY(qreal y);

    //TODO: Slight caching?
    qreal curX() const;
    qreal curSX() const;
    qreal curY() const;
    qreal curSY() const;

    Particle* p;
    ParticleEmitter* e;
    int particleIndex;
    int systemIndex;
};

#endif // PARTICLESYSTEM_H
